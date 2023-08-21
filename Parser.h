/*

    File: Parser.h

    Description:

    A recursive decent parser that takes the tokens from
    the Lexer and uses them to output an abstract syntax
    tree representing the source program.

    Notes:

    The Parser holds a pointer to the Lexer.
    
*/

#pragma once
#include "SyntaxTree/AST.h"
#include "TypeManager.h"
#include "Lexer.h"
#include "SwitchPrint.h"
#include <stack>
#include <typeinfo>
#include <map>
#include <functional>

class TypeManager;
class Parser {
    public:
        Parser(Lexer* lexer, TypeManager* typeManager);
        AST* parse();
    private:
        int currentTokenIndex;
        Lexer* lexer;
        std::vector<Token> tokens;
        TypeManager* typeManager;
        std::vector<std::string> userDefinedTypes;
        std::stack<Operator*> operatorStack;
        std::stack<AST*> operandStack;
        //PARSER FUNCTIONS
        AST* sProgram();
        AST* sStatement();
        AST* sBlock();
        AST* sContext(AST* pTree);
        AST* sAssignment(std::string identifier);
        AST* sRegexSection();
        AST* sForTreeBuilder();
        //EXPRESSION PARSER AND HELPERS
        AST* sExpression();
        void handleOperand();
        void handleOperator();
        void stackPopper();
        //HELPER FUNCTIONS
        Tokens getCurrentToken();
        bool onUserDefinedType();
        std::string& getCurrentLexeme();
        int getCurrentLine();
        //RETURNS BOOL
        bool onStatement();
        bool onDeclaration();
        bool onData();
        bool onOperator();
        bool onOperand();
        bool isCurrentToken(int tokenInt);
        bool onExpressionBreaker();
        //NO RETURN
        void nonAssociativeTypeFlipper(AST* currentTree, Operator* nextTree, int currentTreePrecedence);
        void scan();
        void expect(Tokens token);
};


static std::map<Tokens, std::function<Operator*()>> OperatorMap = {
    { KARAT, []() { return new ExponentTree(); } },
    { STAR, []() { return new MultiplyTree(); } },
    { DIVIDE, []() { return new DivideTree(); } },
    { PLUS, []() { return new AddTree(); } },
    { MINUS, []() { return new SubtractTree(); } },
    { GREATER, []() { return new GreaterTree(); } },
    { GREATER_EQUAL, []() { return new GreaterEqualTree(); } },
    { LESS, []() { return new LessTree(); } },
    { LESS_EQUAL, []() { return new LessEqualTree(); } },
    { EQUAL_EQUAL, []() { return new EqualTree(); } },
    { EXCLAIM_EQUAL, []() { return new NotEqualTree(); } },
    { AND, []() { return new AndTree(); } },
    { OR, []() { return new OrTree(); } },
    { ARROW, []() { return new ArrowOpTree(); } },
    { EQUAL, []() { return new AssignOpTree(); } }
};
        
