#include "SyntaxTree/AST.h"
#ifndef PARSER_H
#define PARSER_H

class Parser {

    public:
        Parser(Lexer* lexer);
        AST* parse();
    private:
        int currentTokenIndex;
        Lexer* lexer;
        std::vector<Token> tokens;

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

        TokenType getCurrentToken();
        Operator* OperatorFactory(TokenType type);
        AST* operatorToASTCaster(Operator* op);
        bool onStatement();
        std::string getCurrentLexeme();
        int getCurrentLine();
        bool onDeclaration();
        bool isCurrentToken(int tokenType);
        bool onOperand();
        bool onOperator();
        bool isData();
        void registerNode(AST* node);
        void scan();
        void expect(TokenType tokenType);
};

#endif