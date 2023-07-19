#include "Parser.h"

Parser::Parser(Lexer* lexer, TypeManager* typeManager) {
    this->tokens = lexer->scanTokens();
    this->typeManager = typeManager;
}
//WRAPPER FOR ABSTRACTION
AST* Parser::parse() {
    return sProgram();
}
  
AST* Parser::sProgram() {
    ProgramTree* pTree = new ProgramTree();
    sContext(pTree);
    return pTree;
}

AST* Parser::sStatement() {
    AST* t;
    if (isCurrentToken(IF)) {
        t = new IfTree();
        scan();
        expect(LEFT_PAREN);
        t->addChild(sExpression());
        expect(RIGHT_PAREN);
        t->addChild(sBlock());
        if (isCurrentToken(ELSE)) {
            scan();
            ElseTree* eTree = new ElseTree();
            eTree->addChild(sBlock());
            t->addChild(eTree);
        }
    } else if (isCurrentToken(WHILE)) {
        t = new WhileTree();
        scan();                
        expect(LEFT_PAREN);
        t->addChild(sExpression());
        expect(RIGHT_PAREN);
        t->addChild(sBlock());
    } else if (isCurrentToken(RETURN)) {
        scan();
        if (onData() || isCurrentToken(IDENTIFIER)) {
            t = new ReturnTree(getCurrentLexeme());
            scan();
        } else {
            t = new ReturnTree();
        }
        expect(SEMICOLON);
        return t;
    } else if (isCurrentToken(IDENTIFIER)) {
        std::string identifier = getCurrentLexeme();
        scan();
        if (isCurrentToken(EQUAL)) { // IF IT IS A REASSIGNMENT OF ALREADY DECLARED VARIABLE
            scan();
            t = sAssignment(identifier);
        } else {
            while (!isCurrentToken(SEMICOLON)) {
                expect(IDENTIFIER);
                

            }
        } /*else { // IF IT IS A FUNCTION CALL
            expect(LEFT_PAREN);
            t = new FunctionCallTree(identifier);
            while (!isCurrentToken(RIGHT_PAREN)) {
                t->addChild(sExpression());
                if (isCurrentToken(COMMA)) {
                    scan();
                }
            }
            scan();
            expect(SEMICOLON);
        }*/
    } 
    return t;
}

AST* Parser::sExpression() {
    ExpressionTree* t = new ExpressionTree();
    std::stack<Operator*> operatorStack;
    std::stack<AST*> operandStack;
    Operator* bottom = new Operator();
    operatorStack.push(bottom);
    #define PRINTEXPRESSION (0x1)
    #ifdef PRINTEXPRESSION
        std::cout << "*** PRINTING EXPRESSION -> LINE: " << getCurrentLine() << " ***" << std::endl;
    #endif
    while (1) {
        #ifdef PRINTEXPRESSION
            std::cout << getCurrentLexeme() << " ";
        #endif
        //
        if (onOperand()) {
            if (isCurrentToken(LEFT_PAREN)){
                scan();
                operandStack.push(sExpression());
                expect(RIGHT_PAREN);
            } else if (isCurrentToken(IDENTIFIER)) {
                IdentifierTree* identTree = new IdentifierTree(getCurrentLexeme());
                operandStack.push(identTree);
                scan();
            } else if (onData()) {
                AST* nTree = this->typeManager->getTypeHandler(getCurrentToken())->getNewTreenode(getCurrentLexeme());
                operandStack.push(nTree);
                scan();
            } else if (isCurrentToken(RIGHT_PAREN) || isCurrentToken(SEMICOLON) || isCurrentToken(COMMA)){
                break;
            } 
        } else {
            std::cerr << "Malformed expression on line " << getCurrentLine() << std::endl;
            exit(1);
        }
        
        if (onOperator()) {
            Operator* opTree = OperatorMap[getCurrentToken()]();
            if (operatorStack.top()->getPrecendence() > opTree->getPrecendence()) {
                int target = opTree->getPrecendence();
                while (operatorStack.top()->getPrecendence() > target) {
                    AST* operand2 = operandStack.top();
                    operandStack.pop();
                    AST* operand1 = operandStack.top();
                    operandStack.pop();
                    int savePrecedence = operatorStack.top()->getPrecendence();
                    AST* operatorHold = dynamic_cast<AST*>(operatorStack.top());
                    operatorStack.pop();
                    /* EDGE CASE */
                    nonAssociativeTypeFlipper(operatorHold, operatorStack.top(), savePrecedence);
                    operatorHold->addChild(operand1);
                    operatorHold->addChild(operand2); 
                    operandStack.push(operatorHold);
                }
            }
            operatorStack.push(opTree);
            scan();
        } else if (isCurrentToken(RIGHT_PAREN) || isCurrentToken(SEMICOLON) || isCurrentToken(COMMA)){
            break;
        } else { 
            std::cerr << "Malformed expression on line " << getCurrentLine() << std::endl;
            exit(1);
        }
    }
    #ifdef PRINTEXPRESSION
        std::cout << "\n";
    #endif 
    if (operandStack.size() < 1) {
        std::cerr << "Malformed expression on line " << getCurrentLine() << std::endl;
        exit(1);
    }
    while (operandStack.size() != 1) {
        AST* operand2 = operandStack.top(); 
        operandStack.pop();
        AST* operand1 = operandStack.top();
        operandStack.pop();
        int savePrecedence = operatorStack.top()->getPrecendence();
        AST* operatorHold = dynamic_cast<AST*>(operatorStack.top());
        operatorStack.pop();
        
        nonAssociativeTypeFlipper(operatorHold, operatorStack.top(), savePrecedence);
        operatorHold->addChild(operand1);
        operatorHold->addChild(operand2);
        operandStack.push(operatorHold);
    }
    t->addChild(operandStack.top());
    operatorStack.pop();
    operatorStack.pop();
    delete bottom;
    bottom = nullptr;
    return t;
}

AST* Parser::sBlock() {
    AST* blockTree =  new BlockTree();
    expect(LEFT_BRACE);
    sContext(blockTree);
    expect(RIGHT_BRACE);
    return blockTree;
}

AST* Parser::sContext(AST* pTree) {
    while (1) {
        if (onStatement()) {
            pTree->addChild(sStatement());
        } else if (onDeclaration()) {
            std::string type  = getCurrentLexeme();
            scan();
            std::string name;
            if (isCurrentToken(LEFT_BRACKET)) {
                scan();
                expect(RIGHT_BRACKET);
                name = getCurrentLexeme();
                AST* arrayDeclaration = new ArrayDeclarationTree(type,name, getCurrentLine());
                pTree->addChild(arrayDeclaration);
                scan();
                if (isCurrentToken(EQUAL)) {
                    scan();
                    arrayDeclaration->addChild(sAssignment(name));
                } 
            } else if (isCurrentToken(LEFT_PAREN)) {
                scan();
                expect(RIGHT_PAREN);
                name = getCurrentLexeme();
                AST* functionDeclaration = new FunctionDeclarationTree(type, name, getCurrentLine());
                pTree->addChild(functionDeclaration);
                scan();
                if (isCurrentToken(EQUAL)) {
                    scan();
                    functionDeclaration->addChild(sAssignment(name));
                } 
            } else if (isCurrentToken(IDENTIFIER)) { 
                name = getCurrentLexeme();
                AST* declaration = new DeclarationTree(type,name,1);
                pTree->addChild(declaration);
                scan();
                if (isCurrentToken(EQUAL)) {
                    scan();
                    declaration->addChild(sAssignment(name));
                } 
            } else {
                std::cerr << "Expected an identifier after the type on line: " << getCurrentLine() << "Instead got: " << getCurrentLexeme() << std::endl;
                exit(1);
            }
            if (isCurrentToken(SEMICOLON)) {
                scan();
            }
        } else {
            break;
        }
    }
    return pTree;
}

AST* Parser::sAssignment(std::string identifier) {
    AST* aTree; //= new AssignTree(identifier, getCurrentLine());
    if (isCurrentToken(LEFT_PAREN)) {
        aTree = new FunctionAssignTree(identifier, getCurrentLine());
        scan();
        while (onDeclaration()) {
            std::string type  = getCurrentLexeme();
            scan();
            std::string name;
            if (isCurrentToken(LEFT_BRACKET)) {
                expect(RIGHT_BRACKET);
                name = getCurrentLexeme();
                aTree->addChild(new ArrayDeclarationTree(type,name, getCurrentLine()));
                scan();
            } else if (isCurrentToken(LEFT_PAREN)) {
                expect(RIGHT_PAREN);
                name = getCurrentLexeme();
                aTree->addChild(new FunctionDeclarationTree(type, name, getCurrentLine()));
                scan();
            } else if (isCurrentToken(IDENTIFIER)) { 
                name = getCurrentLexeme();
                aTree->addChild(new DeclarationTree(type,name,1));
                scan();
            } else {
                std::cerr << "Expected an identifier after the type on line: " << getCurrentLine() << "Instead got: " << getCurrentLexeme() << std::endl;
                exit(1);
            }
            if (!isCurrentToken(COMMA) || isCurrentToken(RIGHT_PAREN)) {
                break;
            } 
            scan();
        }
        expect(RIGHT_PAREN);
        aTree->addChild(sBlock());
        return aTree;
    } else if (isCurrentToken(LEFT_BRACKET)) {
        aTree = new ArrayAssignTree(identifier, getCurrentLine());
        scan();
        while (1) { 
            aTree->addChild(sExpression());
            if (isCurrentToken(RIGHT_BRACKET)) {
                break;
            }
            expect(COMMA);
        }
        expect(SEMICOLON);
        return aTree;
    } else if (onOperand()) {
        aTree = new AssignTree(identifier, getCurrentLine());
        aTree->addChild(sExpression());
        expect(SEMICOLON);
        return aTree;
    } else {
        std::cerr << "Invalid assignment on line: " << getCurrentLine() << std::endl;
        exit(1);
    }

}

TokenType Parser::getCurrentToken() {
    return tokens[currentTokenIndex].type;
}
std::string Parser::getCurrentLexeme() {
    return (tokens[currentTokenIndex].lexeme);
}
int Parser::getCurrentLine() {
    return tokens[currentTokenIndex].line;
}

bool Parser::onStatement() {
    for (int i = 0 ; i < sizeof(Statement)/sizeof(Statement[0]); i++) {
        if (isCurrentToken(Statement[i])) {
            return true;
        }
    }
    return false;
}

bool Parser::onDeclaration() {
    for (int i = 0 ; i < this->typeManager->Declarator.size(); i++) {
        if (isCurrentToken(this->typeManager->Declarator[i])) {
            return true;
        }
    }
    return false;
}
bool Parser::onOperator() {
    for (int i = 0 ; i < sizeof(OperatorArray)/sizeof(OperatorArray[0]); i++) {
        if (isCurrentToken(OperatorArray[i])) {
            return true;
        }
    }
    return false;
}

bool Parser::onOperand() {
    if ( isCurrentToken(RIGHT_PAREN) || isCurrentToken(LEFT_PAREN) || isCurrentToken(IDENTIFIER) || onData() ) {
        return true;
    }
    return false;
}

bool Parser::onData() {
    for (int i = 0 ; i < this->typeManager->Data.size(); i++) {
        if (isCurrentToken(this->typeManager->Data[i])) {
            return true;
        }
    }
    return false;
}
bool Parser::isCurrentToken(int tokenType) {
    if (tokens[currentTokenIndex].type != tokenType) {
        return false;
    }
    return true;
}

void Parser::scan() {
    this->currentTokenIndex++;
}

void Parser::nonAssociativeTypeFlipper(AST* currentTree, Operator* nextTree, int currentTreePrecedence) {

    if (typeid(*nextTree) == typeid(DivideTree) && currentTreePrecedence == 3){
        if (typeid(*currentTree) == typeid(DivideTree)) {
            delete currentTree;
            currentTree = new MultiplyTree(); //9-9+9<9-2
        } else if (typeid(*currentTree) == typeid(MultiplyTree)) {
            delete currentTree;
            currentTree = new DivideTree();
        }
    } else if (typeid(*nextTree) == typeid(SubtractTree) && currentTreePrecedence == 2) {
        if (typeid(*currentTree) == typeid(SubtractTree)) {
            delete currentTree;
            currentTree = new AddTree();
        } else if (typeid(*currentTree) == typeid(AddTree)) {
            delete currentTree;
            currentTree = new SubtractTree();
        }
    }

}

void Parser::expect(TokenType tokenType) {
    if (isCurrentToken(tokenType)) {
        scan();
        return;
    }
    std::cerr << "COMPILATION TERMINATED \n\n";
    std::cerr << "There was a syntax error on line : " << getCurrentLine();
    std::cerr << " Unexpected Token : " << getCurrentLexeme() << std::endl;
    std::cerr << "Compiler expected: " << tokenToStringMap[tokenType] << std::endl;
    exit(1);
}
