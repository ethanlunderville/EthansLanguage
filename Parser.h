/*

    File: Parser.h

    Description:

    A recursive decent parser that takes the tokens from
    the Lexer and uses them to output an abstract syntax
    tree representing the source program based on the CFG
    defined in [].

    Notes:

    The Parser holds a reference to the Lexer.
    
*/
#pragma once
#include "SyntaxTree/AST.h"
#include <stack>
#include "Lexer.h"

class Parser {
    public:
        Parser(Lexer* lexer);
        AST* parse(); // ONLY PUBLIC FUNCTION IS PARSE (ENCAPSULATION, ABSTRACTION)
    private:
        int currentTokenIndex;
        Lexer* lexer;
        std::vector<Token> tokens;
        //PARSER FUNCTIONS
        AST* sProgram();
        AST* sStatement();
        AST* sDeclaration(std::string name, std::string type, short option);
        AST* sFunctionDeclaration(std::string name);
        AST* sExpression();
        AST* sBlock();
        AssignTree* sAssignment(std::string identifier);
        //HELPER FUNCTIONS
        TokenType getCurrentToken();
        std::string getCurrentLexeme();
        int getCurrentLine();
        //RETURNS POINTER TO AST OR OPERATOR
        //NOTE: OPERATOR IS A SUBCLASS OF AST
        Operator* OperatorFactory(TokenType type);
        AST* operatorToASTCaster(Operator* op);
        //RETURNS BOOL
        bool onStatement();
        bool onDeclaration();
        bool onOperand();
        bool onOperator();
        bool isData();
        bool isCurrentToken(int tokenType);
        //NO RETURN
        void nonAssociativeTypeFlipper(AST* currentTree, Operator* nextTree, int currentTreePrecedence);
        void scan();
        void expect(TokenType tokenType);
};