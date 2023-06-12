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
            for (int i = 0 ; i < this->flatTreeHolder.size() ;i++) {
                delete this->flatTreeHolder[i];
                this->flatTreeHolder[i] = nullptr;
            } 
        }
  
        AST* Parser::sProgram() {
            scan();
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
            registerNode(t);
            if (isCurrentToken(IF)) {
                t = new IfTree();
                scan();
                expect(LEFT_PAREN);
                t->addChild(sExpression());
                expect(RIGHT_PAREN);
                t->addChild(sBlock());
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
                } else {
                    t = new ReturnTree();
                }
                scan();
                expect(SEMICOLON);
                return t;
            } else if (isCurrentToken(IDENTIFIER)) {
                std::string name = getCurrentLexeme();
                expect(EQUAL);
                AST* value = sExpression();
                expect(SEMICOLON);
                t = new AssignTree(name);
                t->addChild(value);
            } 
            return t;
        }

        /*
        * IF THE OPTION IS 0 THE DECLARATIONS ARE FUNCTION PARAMETERS
        * IN ALL OTHER CASES IT IS A REGULAR DECLARATION
        */
        AST* Parser::sDeclaration(std::string name, short option) {
            expect(EQUAL);
            std::string value = getCurrentLexeme();
            if (option) {
                expect(SEMICOLON);
            } else {
                if(isCurrentToken(COMMA), isCurrentToken(RIGHT_PAREN)) {
                    scan();
                } else {
                    std::cerr << "Incorrect Syntax for Declaration on line: " << getCurrentLine() << std::endl;
                    exit(1);
                }
            }
            AST* t = new DeclarationTree(name, value);
            registerNode(t);
            return t;
        }

        AST* Parser::sFunctionDeclaration(std::string functionName) {
            AST* t = new FunctionDeclarationTree(functionName);
            registerNode(t);
            expect(LEFT_PAREN);
            while (onDeclaration()) {
                std::string name = getCurrentLexeme();
                t->addChild(sDeclaration(name, 0));
                scan();
            }
            if (isCurrentToken(LEFT_BRACE)) {
                t->addChild(sBlock());
            }
            return t;
        }

        AST* Parser::sExpression() {
            AST* t =  new AST();
            
            return t;
        }

        AST* Parser::sBlock() {
            expect(LEFT_BRACE);
            AST* bTree =  new BlockTree();
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
            std::cerr << "COMPILATION TERMINATED" << std::endl;
            std::cerr << "There was a syntax error on line : " << getCurrentLine() << std::endl;
            exit(1);
        }

//};