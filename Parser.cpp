#include "Parser.h"

Parser::Parser(Lexer* lexer) {
    this->tokens = lexer->scanTokens();
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
            } else if (isCurrentToken(LEFT_PAREN)) {
                pTree->addChild(sFunctionDeclaration(name));
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
        if (isData()) {
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
        t = sAssignment(identifier);
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
        if (isCurrentToken(SEMICOLON)) {
            t = new DeclarationTree(type, identifier, getCurrentLine());
            return t;
        }
        at = sAssignment(identifier);
    } else {
        scan();
        if(!isCurrentToken(RIGHT_PAREN)) {
            expect(COMMA);
        }
    }
    t = new DeclarationTree(type, identifier, getCurrentLine());
    if (at != nullptr) {
        t->addChild(at);
    }
    return t;
}

AST* Parser::sFunctionDeclaration(std::string functionName) {
    AST* t = new FunctionDeclarationTree(functionName);
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
    #ifdef PRINTEXPRESSION
        std::cout << "*** PRINTING EXPRESSION -> LINE: " << getCurrentLine() << " ***" << std::endl;
    #endif
    while (1) {
        #ifdef PRINTEXPRESSION
            std::cout << getCurrentLexeme() << " ";
        #endif
        if(onOperand()) {
            if (isCurrentToken(LEFT_PAREN)){
                scan();
                operandStack.push(sExpression());
                expect(RIGHT_PAREN);
            } 
            if (isCurrentToken(STRING)){
                StringTree* sTree = new StringTree(getCurrentLexeme());
                operandStack.push(sTree);
                scan();
            } else if (isCurrentToken(NUMBER)) {
                NumberTree* nTree = new NumberTree(getCurrentLexeme());
                operandStack.push(nTree);
                scan();
            } else if (isCurrentToken(RIGHT_PAREN)) {
                break;
            }
        } else { 
            std::cerr << "Malformed expression on line " << getCurrentLine() << std::endl;
            exit(1);
        }
        if (onOperator()) {
            Operator* opTree = OperatorFactory(getCurrentToken());
            if (operatorStack.top()->getPrecendence() > opTree->getPrecendence()) {
                int target = opTree->getPrecendence();
                while (operatorStack.top()->getPrecendence() > target) {
                    AST* operand2 = operandStack.top();
                    operandStack.pop();
                    AST* operand1 = operandStack.top();
                    operandStack.pop();
                    int savePrecedence = operatorStack.top()->getPrecendence();
                    AST* operatorHold = operatorToASTCaster(operatorStack.top());
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
        } else if (isCurrentToken(RIGHT_PAREN) || isCurrentToken(SEMICOLON)){
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
        AST* operatorHold = operatorToASTCaster(operatorStack.top());
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
            if (isCurrentToken(EQUAL)) {
                //Option 1 since this is a regular Dec.
                bTree->addChild(sDeclaration(type, name, 1));
            }
            if (isCurrentToken(LEFT_PAREN)) {
                bTree->addChild(sFunctionDeclaration(name));
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
// WAS UNABLE TO TO MAKE THIS WORK WITH POLYMORPHISM 
// AND WAS FORCED TO VIOLATE OPEN CLOSED PRINCIPLE
Operator* Parser::OperatorFactory(TokenType type) {
    Operator* t;
    switch (type) {
        case KARAT:
            t = new ExponentTree();
        break;
        case STAR:
            t = new MultiplyTree();
        break;
        case SLASH:
            t = new DivideTree();
        break;
        case PLUS:
            t = new AddTree();
        break;
        case MINUS:
            t = new SubtractTree();
        break;
        case GREATER:
            t = new GreaterTree();
        break;
        case GREATER_EQUAL:
            t = new GreaterEqualTree();
        break;
        case LESS:
            t = new LessTree();
        break;
        case LESS_EQUAL:
            t = new LessEqualTree();
        break;
        case EQUAL_EQUAL:
            t = new EqualTree();
        break;
        case BANG_EQUAL:
            t = new NotEqualTree();
        break;
        case AND:
            t = new AndTree();
        break;
        case OR:
            t = new OrTree();
        break;
        default:
        std::cerr << "Unexpected type: " << type << ", expected an operator"<< std::endl;
        exit(1);
    }
    return t;
}
// WAS UNABLE TO TO MAKE THIS WORK WITH POLYMORPHISM 
// AND WAS FORCED TO VIOLATE OPEN CLOSED PRINCIPLE
AST* Parser::operatorToASTCaster(Operator *op) {
    AST* t;
        if (typeid(*op) == typeid(ExponentTree)){
            t = (AST*)((ExponentTree*) op);
        } else if (typeid(*op) == typeid(MultiplyTree)){
            t = (AST*)((MultiplyTree*) op);
        } else if (typeid(*op) == typeid(DivideTree)){
            t = (AST*)((DivideTree*) op);
        } else if (typeid(*op) == typeid(AddTree)){
            t = (AST*)((AddTree*) op);
        } else if ( typeid(*op) == typeid(SubtractTree)){
            t = (AST*)((SubtractTree*) op);
        } else if ( typeid(*op) == typeid(GreaterTree)){
            t = (AST*)((GreaterTree*) op);
        } else if ( typeid(*op) == typeid(LessTree)){
            t = (AST*)((LessTree*) op);
        } else if ( typeid(*op) == typeid(GreaterEqualTree)){
            t = (AST*)((GreaterEqualTree*) op);
        } else if ( typeid(*op) == typeid(LessEqualTree)){
            t = (AST*)((LessEqualTree*) op);
        } else if ( typeid(*op) == typeid(EqualTree)){
            t = (AST*)((EqualTree*) op);
        } else if ( typeid(*op) == typeid(NotEqualTree)){
            t = (AST*)((NotEqualTree*) op);
        } else if ( typeid(*op) == typeid(AndTree)){
            t = (AST*)((AndTree*) op);
        } else if ( typeid(*op) == typeid(OrTree)){
            t = (AST*)((OrTree*) op);
        } else {
        std::cerr << "Unexpected type" << std::endl;
        exit(1);
        }
    return t;
}

bool Parser::onStatement() {
    if (isCurrentToken(IF) || isCurrentToken(WHILE) || 
        isCurrentToken(RETURN) || isCurrentToken(IDENTIFIER)) {
        return true;
    }
    return false;
}
bool Parser::onDeclaration() {
    if (isCurrentToken(INT) || isCurrentToken(STRINGTYPE)) {
        return true;
    }
    return false;
}
bool Parser::onOperator() {
    for (int i = 0 ; i < sizeof(Operators)/sizeof(Operators[0]); i++) {
        if (isCurrentToken(Operators[i])) {
            return true;
        }
    }
    return false;
}
bool Parser::onOperand() {
    for (int i = 0 ; i < sizeof(Operands)/sizeof(Operands[0]); i++) {
        if (isCurrentToken(Operands[i])) {
            return true;
        }
    }
    return false;
}
bool Parser::isData() {
    if (isCurrentToken(IDENTIFIER) 
    || isCurrentToken(NUMBER) 
    || isCurrentToken(STRING)) {
        return true;
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
