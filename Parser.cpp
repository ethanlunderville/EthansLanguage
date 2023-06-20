// RECURSIVE DECENT PARSER
#include "Parser.h"
//#define TREENODEDEBUG 0x0

//class Parser {
    //public:
    
        Parser::Parser(Lexer* lexer) {
            this->lexer = lexer;
            this->tokens = this->lexer->scanTokens();
        }

        Parser::~Parser() {
            // Treenodes are all deallocated
            for (int i = 0 ; i < flatTreeHolder.size(); i++) {
                #ifdef TREENODEDEBUG
                   std::cout << this->flatTreeHolder[i] << std::endl; 
                #endif
                delete this->flatTreeHolder[i];
                this->flatTreeHolder[i] = nullptr;
            } 
        }

        //WRAPPER FOR ABSTRACTION
        AST* Parser::parse() {
            return sProgram();
        }
  
        AST* Parser::sProgram() {
            ProgramTree* pTree = new ProgramTree();
            registerNode(pTree);
            while (1) {
                if (onStatement()) {
                    pTree->addChild(sStatement());
                } else if (onDeclaration()) {
                    scan();
                    std::string name = getCurrentLexeme();
                    scan();
                    if (isCurrentToken(EQUAL)) {
                        //Option 1 since this is a regular Dec.
                        pTree->addChild(sDeclaration(name, 1));
                    }
                    if (isCurrentToken(LEFT_PAREN)) {
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
                    registerNode(eTree);
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
                std::string name = getCurrentLexeme();
                t = new AssignTree(name);
                scan();
                expect(EQUAL);
                AST* value = sExpression();
                t->addChild(value);
                expect(SEMICOLON);
            } 
            registerNode(t);
            return t;
        }

        /*
        * IF THE OPTION IS 0 THE DECLARATIONS ARE FUNCTION PARAMETERS
        * IN ALL OTHER CASES IT IS A REGULAR DECLARATION
        */
        AST* Parser::sDeclaration(std::string name, short option) {
            std::string value;
            if (option) {
                expect(EQUAL);
                value = getCurrentLexeme();
                scan();
                expect(SEMICOLON);
            } else {
                scan();
                if(!isCurrentToken(RIGHT_PAREN)) {
                    expect(COMMA);
                }
            }
            DeclarationTree* t = new DeclarationTree(name, value);
            registerNode(t);
            return t;
        }

        AST* Parser::sFunctionDeclaration(std::string functionName) {
            AST* t = new FunctionDeclarationTree(functionName);
            registerNode(t);
            expect(LEFT_PAREN);
            while (onDeclaration()) {
                scan();
                std::string name = getCurrentLexeme();
                t->addChild(sDeclaration(name, 0));
            }
            expect(RIGHT_PAREN);
            if (isCurrentToken(LEFT_BRACE)) {
                t->addChild(sBlock());
            }
            return t;
        }

        AST* Parser::sExpression() {

            ExpressionTree* t = new ExpressionTree();
            registerNode(t);
            std::stack<Operator*> operatorStack;
            std::stack<AST*> operandStack;

            Operator* bottom = new Operator();
            operatorStack.push(bottom);

            while (1) {
                std::cout << getCurrentLexeme() << " BEFORE" << std::endl;
                if(onOperand()) {
                    if (isCurrentToken(LEFT_PAREN)){
                        scan();
                        operandStack.push(sExpression());
                        expect(RIGHT_PAREN);
                        //break;
                    } //else
                    if (isCurrentToken(STRING)){
                        StringTree* sTree = new StringTree(getCurrentLexeme());
                        registerNode(sTree);
                        operandStack.push(sTree);
                        scan();
                    } else if (isCurrentToken(NUMBER)) {
                        NumberTree* nTree = new NumberTree(getCurrentLexeme());
                        registerNode(nTree);
                        operandStack.push(nTree);
                        std::cout << "11111" << std::endl;
                        scan();
                    } else if (isCurrentToken(RIGHT_PAREN)) {
                        break;
                    }
                } else { 
                    std::cerr << "Malformed expression on line " << getCurrentLine() << std::endl;
                    exit(1);
                }
                std::cout << getCurrentLexeme() << " AFTER" << std::endl;
                if (onOperator()) {
                    Operator* opTree = OperatorFactory(getCurrentToken());
                    registerNode(operatorToASTCaster(opTree));
                    if (operatorStack.top()->getPrecendence() > opTree->getPrecendence()) {
                        int target = opTree->getPrecendence();
                        while (operatorStack.top()->getPrecendence() > target) {
                            AST* operand2 = operandStack.top();
                            operandStack.pop();
                            AST* operand1 = operandStack.top();
                            operandStack.pop();
                            AST* operatorHold = operatorToASTCaster(operatorStack.top());
                            operatorStack.pop();
                            operatorHold->addChild(operand1);
                            operatorHold->addChild(operand2); 
                            operandStack.push(operatorHold);
                        }
                    }
                    operatorStack.push(opTree);
                    scan();
                } else if (isCurrentToken(RIGHT_PAREN)){
                    std::cout << "THIS RAN" << std::endl;
                    break;
                } else { 
                    std::cerr << "Malformed expression on line " << getCurrentLine() << std::endl;
                    exit(1);
                }
            }
            while (operandStack.size() != 1) {
                AST* operand2 = operandStack.top(); 
                operandStack.pop();
                AST* operand1 = operandStack.top();
                operandStack.pop();
                AST* operatorHold = operatorToASTCaster(operatorStack.top());
                operatorStack.pop();
                operatorHold->addChild(operand1);
                operatorHold->addChild(operand2);
                operandStack.push(operatorHold);
            }
            
            t->addChild(operandStack.top());
            operatorStack.pop();
            operatorStack.pop();
            delete bottom;
            bottom = nullptr;
            std::cout << "end test:::" << getCurrentLexeme() << std::endl;
            return t;
        }

        AST* Parser::sBlock() {
            expect(LEFT_BRACE);
            AST* bTree =  new BlockTree();
            registerNode(bTree);
            while (1) {
                if (onStatement()) {
                    bTree->addChild(sStatement());
                } else if (onDeclaration()) {
                    scan();
                    std::string name = getCurrentLexeme();
                    scan();
                    if (isCurrentToken(EQUAL)) {
                        //Option 1 since this is a regular Dec.
                        bTree->addChild(sDeclaration(name, 1));
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
        
    //private:

        int currentTokenIndex;
        Lexer* lexer;
        std::vector<Token> tokens;
        std::vector<AST*> flatTreeHolder; // References to AST nodes are held to easily free the nodes

        TokenType Parser::getCurrentToken() {
            return tokens[currentTokenIndex].type;
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
            if (isCurrentToken(INT) || isCurrentToken(BOOL) || isCurrentToken(STRINGTYPE)) {
                return true;
            }
            return false;
        }

        bool Parser::onOperator() {
            if ( isCurrentToken(PLUS) 
            || isCurrentToken(MINUS) 
            || isCurrentToken(STAR)
            || isCurrentToken(SLASH)
            || isCurrentToken(KARAT)
            || isCurrentToken(EQUAL_EQUAL)
            || isCurrentToken(LESS_EQUAL)
            || isCurrentToken(BANG_EQUAL)
            || isCurrentToken(GREATER_EQUAL)
            || isCurrentToken(LESS)
            || isCurrentToken(GREATER)
            ) {
                return true;
            }
            return false;
        }

        bool Parser::onOperand() {
            if ( isCurrentToken(IDENTIFIER)
            || isCurrentToken(NUMBER)
            || isCurrentToken(STRING)
            // FIX THIS
            || isCurrentToken(LEFT_PAREN) //REMOVED DUE TO PARSING ISSUES
            || isCurrentToken(RIGHT_PAREN)
            ) {
                return true;
            }
            return false;
        }

        bool Parser::isData() {
            if (isCurrentToken(IDENTIFIER) 
            || isCurrentToken(TRUE) 
            || isCurrentToken(NUMBER) 
            || isCurrentToken(FALSE) 
            || isCurrentToken(STRING)) {
                return true;
            }
            return false;
        }

        std::string Parser::getCurrentLexeme() {
            return (tokens[currentTokenIndex].lexeme);
        }

        int Parser::getCurrentLine() {
            return tokens[currentTokenIndex].line;
        }

        bool Parser::isCurrentToken(int tokenType) {
            if (tokens[currentTokenIndex].type != tokenType) {
                return false;
            }
            return true;
        }

        void Parser::registerNode(AST* node) {
            flatTreeHolder.push_back(node);
        }

        void Parser::scan() {
            this->currentTokenIndex++;
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

//};