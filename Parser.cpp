// RECURSIVE DECENT PARSER

#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.cpp"

class Parser {

    public:
    
        Parser(Lexer* lexer) {
            this->lexer = lexer;
            this->tokens = this->lexer->scanTokens();
        }

        ~Parser() {
            for (int i = 0 ; i < this->tokens.size() ;i++) {
                delete this->tokens[i];
                this->tokens[i] = nullptr;
            } 
        }

        AST* sExpression() {
            AST* t =  new AST();
            
            return t;
        }

        AST* sBlock() {
            AST* t =  new AST();
            
            return t;
        }

        AST* sStatement() {
            AST* t;
            registerNode(t);
            if (isCurrentToken(IF)) {
                scan();
                t->addChild(sExpression());
                T->addChild(sBlock());
            } else if (isCurrentToken(WHILE)) {
                scan();
            } else if (isCurrentToken(RETURN)) {
                scan();
            } else if (isCurrentToken(IDENTIFIER)) {
                scan();
            } 
            return t;
        }

        AST* sProgram() {
            scan();
            ProgramTree* pTree = new ProgramTree();
            registerNode(pTree);
            bool onState;
            bool onDecl;
            while (onState = onStatement() || onDecl = onDeclaration()) {
                if (onState) {
                    pTree->addChild(sStatement());
                }
                if (onDecl) {
                    pTree->addChild(sStatement());
                    //pTree->addChild(sDeclaration());
                }
            }
            return pTree;
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
            if (isCurrentToken(INT) || isCurrentToken(BOOLEAN) ) {
                return true;
            }
            return false;
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
            std::cerr << "There was a syntax error on line : " << tokens[currentTokenIndex].line << std::endl;
            exit(1);
        }

};