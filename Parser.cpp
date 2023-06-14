// RECURSIVE DECENT PARSER
#include "Parser.h"

//class Parser {
    //public:
    
        Parser::Parser(Lexer* lexer) {
            this->lexer = lexer;
            this->tokens = this->lexer->scanTokens();
        }

        Parser::~Parser() {
            // Treenodes are all deallocated
            for (int i = 0 ; i < flatTreeHolder.size(); i++) {
                std::cout << this->flatTreeHolder[i] << std::endl;
                delete this->flatTreeHolder[i];
                this->flatTreeHolder[i] = nullptr;
            } 
        }

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
                registerNode(t);
                scan();
                expect(LEFT_PAREN);
                t->addChild(sExpression());
                expect(RIGHT_PAREN);
                t->addChild(sBlock());
            } else if (isCurrentToken(WHILE)) {
                t = new WhileTree();
                registerNode(t);
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
                registerNode(t);
                return t;
            } else if (isCurrentToken(IDENTIFIER)) {
                std::string name = getCurrentLexeme();
                scan();
                expect(EQUAL);
                AST* value = sExpression();
                expect(SEMICOLON);
                t = new AssignTree(name);
                registerNode(t);
                t->addChild(value);
            } 
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
                    std::cout << "BEFORE" << std::endl;
                    
                    expect(COMMA);
                    std::cout << "AFTER"<< std::endl;
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
            while (onExpressionToken()) {
                scan();
            }
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

        bool Parser::onExpressionToken() {
            if ( isCurrentToken(PLUS) 
            || isCurrentToken(MINUS) 
            || isCurrentToken(STAR)
            || isCurrentToken(SLASH)
            || isCurrentToken(IDENTIFIER)
            || isCurrentToken(NUMBER)
            || isCurrentToken(EQUAL_EQUAL)
            || isCurrentToken(LESS_EQUAL)
            || isCurrentToken(BANG_EQUAL)
            || isCurrentToken(GREATER_EQUAL)
            || isCurrentToken(LESS)
            || isCurrentToken(GREATER)
            // FIX THIS
            //|| isCurrentToken(LEFT_PAREN) //REMOVED DUE TO PARSING ISSUES
            //|| isCurrentToken(RIGHT_PAREN)
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