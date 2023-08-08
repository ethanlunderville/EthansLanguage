#include "Parser.h"

Parser::Parser(Lexer* lexer, TypeManager* typeManager) : tokens(lexer->scanTokens()) , typeManager(typeManager) {}

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
        t = new ReturnTree();
        if (!isCurrentToken(SEMICOLON)) {
            t->addChild(sExpression());
        }
        expect(SEMICOLON);
        return t;
    } else if (isCurrentToken(IDENTIFIER) && !onUserDefinedType()) {
        t = sExpression();
        expect(SEMICOLON);
    } 
    return t;
}          

AST* Parser::sExpression() {
    AST* t = new ExpressionTree(getCurrentLine());
    std::stack<Operator*> operatorStack;
    std::stack<AST*> operandStack;
    Operator* bottom = new Operator();
    operatorStack.push(bottom);
    #ifdef PRINTEXPRESSION
        std::cout << "*** PRINTING EXPRESSION -> LINE: " << getCurrentLine() << " ***" << std::endl;
    #endif
    while (1) {
        #ifdef PRINTEXPRESSION
            std::cout << getCurrentLexeme() << " ";
        #endif
        if (onOperand()) {
            if (isCurrentToken(LEFT_PAREN)){
                scan();
                operandStack.push(sExpression());
                expect(RIGHT_PAREN);
            } else if (isCurrentToken(IDENTIFIER)) {
                AST* identTree;
                std::string identifier = getCurrentLexeme();
                scan();
                if (isCurrentToken(LEFT_BRACKET)) {
                    identTree = new ArrayAccessTree(identifier);
                    scan();
                    identTree->addChild(sExpression());
                    expect(RIGHT_BRACKET);
                } else if (isCurrentToken(LEFT_PAREN)) {
                    scan();
                    identTree = new FunctionCallTree(identifier);
                    while (!isCurrentToken(RIGHT_PAREN)) {
                        identTree->addChild(sExpression());
                        if (isCurrentToken(COMMA)) {
                            scan();
                        }
                    }
                    scan();
                } else {
                    identTree = new IdentifierTree(identifier);
                }
                operandStack.push(identTree);
            } else if (onData()) {
                AST* nTree = dynamic_cast<PrimitiveType*>(this->typeManager->getTypeHandler(getCurrentToken()))->getNewTreenode(getCurrentLexeme());
                operandStack.push(nTree);
                scan();
            } else if (isCurrentToken(RIGHT_PAREN) || isCurrentToken(SEMICOLON) || isCurrentToken(COMMA) || isCurrentToken(RIGHT_BRACKET)){
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
        } else if (isCurrentToken(RIGHT_PAREN) || isCurrentToken(SEMICOLON) || isCurrentToken(COMMA) || isCurrentToken(RIGHT_BRACKET)){
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
    // ENSURE THAT R-VALUE NODES ARE ASSIGNABLE  
    if (typeid(*(operandStack.top())) == typeid(AssignOpTree)) {
        Assignable* expressionTop = dynamic_cast<Assignable*>(operandStack.top()->getChildren()[1]);
        if (expressionTop == nullptr) {
            AssignOpTree* aOpTree = dynamic_cast<AssignOpTree*>(operandStack.top());
            ExpressionTree* exprTree = new ExpressionTree(getCurrentLine());
            exprTree->addChild(aOpTree->getChildren()[1]);
            std::vector<AST*>& assignOpKidRef = aOpTree->getChildren();
            assignOpKidRef[1] = exprTree;
        }
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
        if (tokens.size() == currentTokenIndex) {
            break;
        } else if (getCurrentLexeme().compare("print") == 0) {
            PrintTree* printTree = new PrintTree();
            scan();
            expect(LEFT_PAREN);
            printTree->addChild(sExpression());
            expect(RIGHT_PAREN);
            expect(SEMICOLON);
            pTree->addChild(printTree);
        } else if (onStatement()) {
            pTree->addChild(sStatement());
        } else if ( isCurrentToken(STRUCT) || onDeclaration()) {
            std::string type = getCurrentLexeme();
            scan();
            std::string name;
            if (isCurrentToken(LEFT_BRACKET)) {
                scan();
                expect(RIGHT_BRACKET);
                name = getCurrentLexeme();
                AST* arrayDeclaration = new ArrayDeclarationTree(type,name, getCurrentLine());
                pTree->addChild(arrayDeclaration);
                scan();
            } else if (isCurrentToken(LEFT_PAREN)) {
                scan();
                expect(RIGHT_PAREN);
                name = getCurrentLexeme();
                AST* functionDeclaration = new FunctionDeclarationTree(type, name, getCurrentLine());
                pTree->addChild(functionDeclaration);
                scan();
            } else if (isCurrentToken(IDENTIFIER)) { 
                name = getCurrentLexeme();
                AST* declaration;
                if (type.compare("struct") == 0) { // HARDCODED SINCE IT IS A VERY SPECIAL CASE
                    declaration = new StructDeclarationTree(type, name, getCurrentLine());
                    this->userDefinedTypes.push_back(name);
                    while (!isCurrentToken(LEFT_BRACE)) {
                        scan();
                    }
                    declaration->addChild(sBlock());
                    pTree->addChild(declaration);
                    continue;
                } else {
                    declaration = new DeclarationTree(type,name,getCurrentLine());
                }
                pTree->addChild(declaration);
                scan();
            } else {
                std::cerr << "Expected an identifier after the type on line: " << getCurrentLine() << " Instead got: " << getCurrentLexeme() << std::endl;
                exit(1);
            }
            if (isCurrentToken(EQUAL)) {
                scan();
                AST* assignOp = sAssignment(name);
                ExpressionTree* expressionForParent = new ExpressionTree(getCurrentLine());
                assignOp->prependToChildren(new IdentifierTree(name));
                expressionForParent->addChild(assignOp);
                pTree->addChild(expressionForParent);                    
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
    AST* aTree = new AssignOpTree();
    if (isCurrentToken(NEW)) {
        scan();
        if (onUserDefinedType()) {
            aTree->addChild(new StructAssignTree(getCurrentLexeme() ,getCurrentLine()));
            scan();
            expect(LEFT_PAREN);
            expect(RIGHT_PAREN);
            expect(SEMICOLON);
            return aTree;
        } else {
            std::cerr << "Unrecognized struct name: " << getCurrentLexeme() << "on line: " << getCurrentLine();
            exit(1);
        }
    } else if (isCurrentToken(LEFT_BRACE)) {
        StructAssignTree* sAssignTree = new StructAssignTree(identifier, getCurrentLine());
        sAssignTree->addChild(sBlock());
        aTree->addChild(sAssignTree);
        return aTree;
    } else if (isCurrentToken(LEFT_PAREN)) {
        scan();
        FunctionAssignTree* functionAssignTree = new FunctionAssignTree(getCurrentLexeme(), getCurrentLine());
        while (onDeclaration()) {
            std::string type = getCurrentLexeme();
            scan();
            std::string name;
            if (isCurrentToken(LEFT_BRACKET)) {
                scan();
                expect(RIGHT_BRACKET);
                name = getCurrentLexeme();
                functionAssignTree->addChild(new ArrayDeclarationTree(type,name, getCurrentLine()));
                scan();
            } else if (isCurrentToken(LEFT_PAREN)) {
                scan();
                expect(RIGHT_PAREN);
                name = getCurrentLexeme();
                functionAssignTree->addChild(new FunctionDeclarationTree(type, name, getCurrentLine()));
                scan();
            } else if (isCurrentToken(IDENTIFIER)) { 
                name = getCurrentLexeme();
                functionAssignTree->addChild(new DeclarationTree(type,name,1));
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
        expect(EQUALARROW);
        functionAssignTree->addChild(sBlock());
        aTree->addChild(functionAssignTree);
        return aTree;
    } else if (isCurrentToken(LEFT_BRACKET)) {
        ArrayAssignTree* arrayAssignTree = new ArrayAssignTree(getCurrentLexeme(), getCurrentLine());
        scan();
        while (1) { 
            arrayAssignTree->addChild(sExpression());
            if (isCurrentToken(RIGHT_BRACKET)) {
                scan();
                break;
            }
            expect(COMMA);
        }
        expect(SEMICOLON);
        aTree->addChild(arrayAssignTree);
        return aTree;
    } else if (onOperand()) {
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
std::string& Parser::getCurrentLexeme() {
    return tokens[currentTokenIndex].lexeme;
}
int Parser::getCurrentLine() {
    return tokens[currentTokenIndex].line;
}

bool Parser::onUserDefinedType() { 
    if (this->currentTokenIndex < this->tokens.size()) {
        std::string lexeme = getCurrentLexeme();
        for (int i = 0 ; i < this->userDefinedTypes.size() ; i++) {
            if (this->userDefinedTypes[i].compare(lexeme) == 0) {
                return true;
            }
        }
        return false;
    } else {
        return false;
    }
}

bool Parser::onStatement() {
    if (onUserDefinedType()) {
        return false;
    }  
    for (int i = 0 ; i < sizeof(Statement)/sizeof(Statement[0]); i++) {
        if (isCurrentToken(Statement[i])) {
            return true;
        }
    }
    return false;
}

bool Parser::onDeclaration() {
    if (isCurrentToken(STRUCT) || onUserDefinedType()) {
        return true;
    }
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
