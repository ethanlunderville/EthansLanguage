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
    while (1) {
        if (onStatement()) {
            pTree->addChild(sStatement());
        } else if (onDeclaration()) {
            std::string type  = getCurrentLexeme();
            scan();
            std::string name = getCurrentLexeme();
            scan();
            if (isCurrentToken(EQUAL) || isCurrentToken(SEMICOLON)) {
                //Option 1 since this is a regular Dec.
                pTree->addChild(sDeclaration(type,name,1));
            } 
            if (isCurrentToken(LEFT_PAREN)) {
                pTree->addChild(sFunctionDeclaration(type,name));
            }
        } else {
            break;
        }
    }
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
        if (isCurrentToken(EQUAL)) {
            t = sAssignment(identifier);
        } else {
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
        }
    } 
    return t;
}

/*
* IF THE OPTION IS 0 THE DECLARATIONS ARE FUNCTION PARAMETERS
* IN ALL OTHER CASES IT IS A REGULAR DECLARATION
*/
AST* Parser::sDeclaration( std::string type , std::string identifier, short option) {
    std::string value;
    DeclarationTree* t;
    AssignTree* at = nullptr;
    if (option) {
        if (isCurrentToken(SEMICOLON)) { //DECLARATION WITHOUT ASSIGNMENT 
            scan();
            t = new DeclarationTree(type, identifier, getCurrentLine());
            return t;
        }
        // DECLARATION WITH ASSIGNMENT
        t = new DeclarationTree(type, identifier, getCurrentLine());
        t->addChild(sAssignment(identifier));
        
    } else { // FUNCITON PARAMETERS
        scan();
        if(!isCurrentToken(RIGHT_PAREN)) {
            expect(COMMA);
        }
        t = new DeclarationTree(type, identifier, getCurrentLine());
    }
    return t;
}

AST* Parser::sFunctionDeclaration(std::string type, std::string name) {
    AST* t = new FunctionDeclarationTree(type, name, getCurrentLine());
    expect(LEFT_PAREN);
    while (onDeclaration()) {
        std::string type = getCurrentLexeme();
        scan();
        std::string identifier = getCurrentLexeme();
        t->addChild(sDeclaration(type, identifier , 0));
    }
    expect(RIGHT_PAREN);
    if (isCurrentToken(LEFT_BRACE)) {
        t->addChild(sBlock());
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
            } 
            if (isCurrentToken(IDENTIFIER)) {
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
    expect(LEFT_BRACE);
    AST* bTree =  new BlockTree();
    while (1) {
        if (onStatement()) {
            bTree->addChild(sStatement());
        } else if (onDeclaration()) {
            std::string type = getCurrentLexeme();
            scan();
            std::string name = getCurrentLexeme();
            scan();
            if (isCurrentToken(EQUAL) || isCurrentToken(SEMICOLON)) {
                //Option 1 since this is a regular Dec.
                bTree->addChild(sDeclaration(type, name, 1));
            }
            if (isCurrentToken(LEFT_PAREN)) {
                bTree->addChild(sFunctionDeclaration(type, name));
            }
        } else {
            break;
        }
    }
    expect(RIGHT_BRACE);
    return bTree;
}

AssignTree* Parser::sAssignment(std::string identifier) {
    AssignTree* t = new AssignTree(identifier, getCurrentLine());
    expect(EQUAL);
    AST* value = sExpression();
    t->addChild(value);
    expect(SEMICOLON);
    return t;
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
