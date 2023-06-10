// RECURSIVE DECENT PARSER

#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.cpp"
#include "Parser.h"

class Parser {

    public:
    
        Parser(Lexer* lexer) {
            this->lexer = lexer;
            this->tokens = this->lexer->scanTokens();
        }

        ~Parser() {
            // Treenodes are all deallocated
            for (int i = 0 ; i < this->flatTreeHolder.size() ;i++) {
                delete this->flatTreeHolder[i];
                this->flatTreeHolder[i] = nullptr;
            } 
        }
  
        AST* sProgram() {
            scan();
            ProgramTree* pTree = new ProgramTree();
            registerNode(pTree);
            while (1) {
                if (onStatement()) {
                    pTree->addChild(sStatement());
                } else if (onDeclaration()) {
                    scan()
                    std::string* name = getCurrentLexeme();
                    scan()
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

        AST* sStatement() {
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
                t = new ReturnTree(getCurrentLexeme());
                expect(SEMICOLON);
            } else if (isCurrentToken(IDENTIFIER)) {
                std::string* name = getCurrentLexeme();
                expect(EQUAL);
                std::string* value = getCurrentLexeme();
                expect(SEMICOLON);
                t = new AssignTree(name, value);
            } 
            return t;
        }

        /*
        * IF THE OPTION IS 0 THE DECLARATIONS ARE FUNCTION PARAMETERS
        * IN ALL OTHER CASES IT IS A REGULAR DECLARATION
        */
        AST* sDeclaration(std::string* name, short option) {
            expect(EQUAL);
            std::string* value = getCurrentLexeme();
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

        AST* sFunctionDeclaration(std::string* functionName) {
            AST* t = new FunctionDeclarationTree(functionName);
            registerNode(t);
            expect(LEFT_PAREN);
            while (onDeclaration()) {
                std::string* name = getCurrentLexeme();
                t->addChild(sDeclaration(name, 0));
                scan();
            }
            if (isCurrentToken(LEFT_BRACE)) {
                t->addChild(sBlock());
            }
            return t;
        }

        AST* sExpression() {
            AST* t =  new AST();
            
            return t;
        }

        AST* sBlock() {
            expect(LEFT_BRACE);
            AST* bTree =  new BlockTree();
            while (1) {
                if (onStatement()) {
                    pTree->addChild(sStatement());
                } else if (onDeclaration()) {
                    scan()
                    std::string* name = getCurrentLexeme();
                    scan()
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
            return bTree
            expect(RIGHT_BRACE);
            return t;
        }
        
    private:

        int currentTokenIndex;
        Lexer* lexer;
        std::vector<Token> tokens;
        std::vector<AST*> flatTreeHolder; // References to AST nodes are held to easily free the nodes

        bool onStatement() {
            if (isCurrentToken(IF) || isCurrentToken(WHILE) || 
                isCurrentToken(RETURN) || isCurrentToken(IDENTIFIER)) {
                return true;
            }
            return false;
        }

        bool onDeclaration() {
            if (isCurrentToken(INT) || isCurrentToken(BOOL) || isCurrentToken(STRINGTYPE)) {
                return true;
            }
            return false;
        }

        std::string* getCurrentLexeme() {
            return &(tokens[currentTokenIndex].lexeme);
        }

        int getCurrentLine() {
            return tokens[currentTokenIndex].line;
        }

        bool isCurrentToken(int tokenType) {
            if (tokens[currentTokenIndex].type != tokenType) {
                return false;
            }
            return true;
        }

        void registerNode(AST* node) {
            flatTreeHolder.push_back(node);
        }

        void scan() {
            this->currentTokenIndex++;
        }

        void expect(TokenType tokenType) {
            if (isCurrentToken(tokenType)) {
                scan();
                return;
            }
            std::cerr << "COMPILATION TERMINATED" << std::endl;
            std::cerr << "There was a syntax error on line : " << getCurrentLine() << std::endl;
            exit(1);
        }

};