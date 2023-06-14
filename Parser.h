#include "SyntaxTree/AST.h"
#ifndef PARSER_H
#define PARSER_H

class Parser {

    public:
    
        Parser(Lexer* lexer);
        ~Parser();
        AST* parse();

        AST* sProgram();
        
        AST* sStatement();
        AST* sIf();
        AST* sElse();
        AST* sWhile();
        AST* sReturn();
        AST* sExpression();
        AST* sBlock();

        AST* sDeclaration(std::string name, short option);
        AST* sFunctionDeclaration(std::string name);
        AST* sFunction();
        AST* sAssign();

        AST* sAdd();
        AST* sDivide();
        AST* sMultiply();
        AST* sSubtract();

    private:

        int currentTokenIndex;
        Lexer* lexer;
        std::vector<Token> tokens;
        std::vector<AST*> flatTreeHolder; // References to AST nodes are held to easily free the nodes

        bool onStatement();
        std::string getCurrentLexeme();
        int getCurrentLine();
        bool onDeclaration();
        bool isCurrentToken(int tokenType);
        bool onExpressionToken();
        bool isData();
        void registerNode(AST* node);
        void scan();
        void expect(TokenType tokenType);

};

#endif