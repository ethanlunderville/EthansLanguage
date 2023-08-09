/*

    File: ASTVisitor.h

    Description:

    This is the header file for the ASTVisitor and all of
    its subclasses. 
    
*/

#pragma once
#include "TypeManager.h"
#include "SwitchPrint.h"
#include "Library/Builtins.h"
#include <stack>
#include <iostream>

class AST;

class ContextManager;
/*Abstract*/ class ASTVisitor {
    public:
        virtual void visitChildren(AST* astree)=0;
        virtual void visitArrayAccessTree(AST* astree)=0;
        virtual void visitFunctionAssignTree(AST* astree)=0;
        virtual void visitArrayAssignTree(AST* astree)=0;
        virtual void visitStructAssignTree(AST* astree)=0;
        virtual void visitIfTree(AST* astree)=0;
        virtual void visitExpressionTree(AST* astree)=0;
        virtual void visitDivideTree(AST* astree)=0;
        virtual void visitMultiplyTree(AST* astree)=0;
        virtual void visitAddTree(AST* astree)=0;
        virtual void visitSubtractTree(AST* astree)=0;
        virtual void visitExponentTree(AST* astree)=0;
        virtual void visitDeclarationTree(AST* astree)=0;
        virtual void visitBlockTree(AST* astree)=0;
        virtual void visitReturnTree(AST* astree)=0;
        virtual void visitProgramTree(AST* astree)=0;
        virtual void visitFunctionDeclarationTree(AST* astree)=0;
        virtual void visitArrayDeclarationTree(AST* astree)=0;
        virtual void visitStructDeclarationTree(AST* astree)=0;
        virtual void visitWhileTree(AST* astree)=0;
        virtual void visitElseTree(AST* astree)=0;
        virtual void visitStringTree(AST* astree)=0;
        virtual void visitNumberTree(AST* astree)=0;
        virtual void visitIdentifierTree(AST* astree)=0;
        virtual void visitGreaterTree (AST* astree)=0;
        virtual void visitGreaterEqualTree (AST* astree)=0;
        virtual void visitLessTree (AST* astree)=0;
        virtual void visitLessEqualTree (AST* astree)=0;
        virtual void visitEqualTree (AST* astree)=0;
        virtual void visitNotEqualTree (AST* astree)=0;
        virtual void visitAndTree (AST* astree)=0;
        virtual void visitOrTree (AST* astree)=0;
        virtual void visitFunctionCallTree (AST* astree)=0;
        virtual void visitAssignOpTree (AST* astree)=0;
        virtual void visitArrowOpTree (AST* astree)=0;
        virtual void visitPrintTree (AST* astree)=0;
};

class ASTPrintVisitor: public ASTVisitor {
    public:
        ASTPrintVisitor();
        void visitChildren(AST* astree) override;
        void visitArrayAccessTree (AST* astree) override;
        void visitFunctionAssignTree (AST* astree) override;
        void visitArrayAssignTree (AST* astree) override;
        void visitStructAssignTree (AST* astree) override;
        void visitIfTree (AST* astree) override;
        void visitExpressionTree (AST* astree) override;
        void visitDivideTree (AST* astree) override;
        void visitMultiplyTree (AST* astree) override;
        void visitAddTree (AST* astree) override;
        void visitSubtractTree (AST* astree) override;
        void visitDeclarationTree (AST* astree) override;
        void visitBlockTree (AST* astree) override;
        void visitReturnTree (AST* astree) override;
        void visitProgramTree (AST* astree) override;
        void visitFunctionDeclarationTree (AST* astree) override;
        void visitArrayDeclarationTree (AST* astree) override;
        void visitStructDeclarationTree (AST* astree) override;
        void visitWhileTree (AST* astree) override;
        void visitElseTree (AST* astree) override;
        void visitStringTree(AST* astree) override;
        void visitNumberTree(AST* astree) override;
        void visitIdentifierTree(AST* astree) override;
        void visitExponentTree(AST* astree) override;
        void visitGreaterTree (AST* astree) override;
        void visitGreaterEqualTree (AST* astree) override;
        void visitLessTree (AST* astree) override;
        void visitLessEqualTree (AST* astree) override;
        void visitEqualTree (AST* astree) override;
        void visitNotEqualTree (AST* astree) override;
        void visitAndTree (AST* astree) override;
        void visitOrTree (AST* astree) override;
        void visitFunctionCallTree(AST* astree) override;
        void visitAssignOpTree (AST* astree) override;
        void visitArrowOpTree(AST* astree) override;
        void visitPrintTree(AST* astree) override;

        void printIndent();
        void printer(const std::string& type, AST* node, const std::string& symbol);
        void printer(const std::string& type, AST* node);
        void printer(const std::string& type, AST* node, const std::vector<std::string>& propList);

    private:
        int indent;
        int lineNum;
};

class ASTDeallocationVisitor: public ASTVisitor {
    public:
        ASTDeallocationVisitor();
        void visitChildren(AST* astree) override;
        void visitArrayAccessTree (AST* astree) override;
        void visitFunctionAssignTree (AST* astree) override;
        void visitArrayAssignTree (AST* astree) override;
        void visitStructAssignTree (AST* astree) override;
        void visitIfTree (AST* astree) override;
        void visitExpressionTree (AST* astree) override;
        void visitDivideTree (AST* astree) override;
        void visitMultiplyTree (AST* astree) override;
        void visitAddTree (AST* astree) override;
        void visitSubtractTree (AST* astree) override;
        void visitDeclarationTree (AST* astree) override;
        void visitBlockTree (AST* astree) override;
        void visitReturnTree (AST* astree) override;
        void visitProgramTree (AST* astree) override;
        void visitFunctionDeclarationTree (AST* astree) override;
        void visitArrayDeclarationTree (AST* astree) override;
        void visitStructDeclarationTree (AST* astree) override;
        void visitWhileTree (AST* astree) override;
        void visitElseTree (AST* astree) override;
        void visitStringTree(AST* astree) override;
        void visitIdentifierTree(AST* astree) override;
        void visitNumberTree(AST* astree) override;
        void visitExponentTree(AST* astree) override;
        void visitGreaterTree (AST* astree) override;
        void visitGreaterEqualTree (AST* astree) override;
        void visitLessTree (AST* astree) override;
        void visitLessEqualTree (AST* astree) override;
        void visitEqualTree (AST* astree) override;
        void visitNotEqualTree (AST* astree) override;
        void visitAndTree (AST* astree) override;
        void visitOrTree (AST* astree) override;
        void visitFunctionCallTree(AST* astree) override;
        void visitAssignOpTree (AST* astree) override;
        void visitArrowOpTree(AST* astree) override;
        void visitPrintTree(AST* astree) override;

        void deAllocateChildren();
        void deallocate(AST* node);

    private:
        int currentNodeNum;
};

class ASTInterpreter: public ASTVisitor {
    public:
        ASTInterpreter(TypeManager* typeManager);
        ASTInterpreter(TypeManager* typeManager, ContextManager* contextManager);
        ASTInterpreter(ContextManager* cm);
        ~ASTInterpreter();
        void visitChildren(AST* astree) override;
        void visitArrayAccessTree (AST* astree) override;
        void visitFunctionAssignTree (AST* astree) override;
        void visitArrayAssignTree (AST* astree) override;
        void visitStructAssignTree (AST* astree) override;
        void visitIfTree (AST* astree) override;
        void visitExpressionTree (AST* astree) override;
        void visitDivideTree (AST* astree) override;
        void visitMultiplyTree (AST* astree) override;
        void visitAddTree (AST* astree) override;
        void visitSubtractTree (AST* astree) override;
        void visitDeclarationTree (AST* astree) override;
        void visitBlockTree (AST* astree) override;
        void visitReturnTree (AST* astree) override;
        void visitProgramTree (AST* astree) override;
        void visitFunctionDeclarationTree (AST* astree) override;
        void visitArrayDeclarationTree (AST* astree) override;
        void visitStructDeclarationTree (AST* astree) override;
        void visitWhileTree (AST* astree) override;
        void visitElseTree (AST* astree) override;
        void visitStringTree(AST* astree) override;
        void visitNumberTree(AST* astree) override;
        void visitIdentifierTree(AST* astree) override;
        void visitExponentTree(AST* astree) override;
        void visitGreaterTree (AST* astree) override;
        void visitGreaterEqualTree (AST* astree) override;
        void visitLessTree (AST* astree) override;
        void visitLessEqualTree (AST* astree) override;
        void visitEqualTree (AST* astree) override;
        void visitNotEqualTree (AST* astree) override;
        void visitAndTree (AST* astree) override;
        void visitOrTree (AST* astree) override;
        void visitFunctionCallTree(AST* astree) override;
        void visitAssignOpTree (AST* astree) override;
        void visitArrowOpTree(AST* astree) override;
        void visitPrintTree(AST* astree) override;

        void visitLValArrowOpTree(AST* astree);
        void visitLValArrayAccessTree(AST* astree);
        void visitLValIdentifierTree(AST* astree);
        void visitFunctionChildren(AST* astree);

        bool temporary;

    private:
        ContextManager* contextManager;
        TypeManager* typeManager;
        ASTChecker* lValBubbler;
        bool returned;
        FunctionCallTree* currentFuncPtr;
        std::stack<SymbolTable*> structScoper;
        std::stack<FunctionCallTree*> callStack;
};

class ASTChecker: public ASTVisitor {
    public:
        ASTChecker(TypeManager* typeManager);
        ~ASTChecker();
        //INHERITED FUNCTIONS
        void visitChildren(AST* astree) override;
        void visitArrayAccessTree (AST* astree) override;
        void visitFunctionAssignTree (AST* astree) override;
        void visitArrayAssignTree (AST* astree) override;
        void visitStructAssignTree (AST* astree) override;
        void visitIfTree (AST* astree) override;
        void visitExpressionTree (AST* astree) override;
        void visitDivideTree (AST* astree) override;
        void visitMultiplyTree (AST* astree) override;
        void visitAddTree (AST* astree) override;
        void visitSubtractTree (AST* astree) override;
        void visitDeclarationTree (AST* astree) override;
        void visitBlockTree (AST* astree) override;
        void visitReturnTree (AST* astree) override;
        void visitProgramTree (AST* astree) override;
        void visitFunctionDeclarationTree (AST* astree) override;
        void visitArrayDeclarationTree (AST* astree) override;
        void visitStructDeclarationTree (AST* astree) override;
        void visitWhileTree (AST* astree) override;
        void visitElseTree (AST* astree) override;
        void visitStringTree(AST* astree) override;
        void visitNumberTree(AST* astree) override;
        void visitIdentifierTree(AST* astree) override;
        void visitExponentTree(AST* astree) override;
        void visitGreaterTree (AST* astree) override;
        void visitGreaterEqualTree (AST* astree) override;
        void visitLessTree (AST* astree) override;
        void visitLessEqualTree (AST* astree) override;
        void visitEqualTree (AST* astree) override;
        void visitNotEqualTree (AST* astree) override;
        void visitAndTree (AST* astree) override;
        void visitOrTree (AST* astree) override;
        void visitFunctionCallTree(AST* astree) override;
        void visitAssignOpTree (AST* astree) override;
        void visitArrowOpTree(AST* astree) override;
        void visitPrintTree(AST* astree) override;

        void visitLValArrowOpTree(AST* astree);
        void visitLValArrayAccessTree(AST* astree);
        void visitLValIdentifierTree(AST* astree);
        bool numberCheck(AST* astree);

    private:
        ContextManager* contextManager;
        TypeManager* typeManager;
        std::stack<SymbolTable*> structScoper;
        std::vector<ReturnTree*> returnTrees;
};
