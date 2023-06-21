#include <iostream>
#ifndef ASTVISITOR_H
#define ASTVISITOR_H
class AST;
class ASTVisitor {
    public:
        virtual void visitChildren(AST* astree)=0;
        virtual void visitAssignTree(AST* astree)=0;
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
        virtual void visitFunctionTree(AST* astree)=0;
        virtual void visitFunctionDeclarationTree(AST* astree)=0;
        virtual void visitWhileTree(AST* astree)=0;
        virtual void visitElseTree(AST* astree)=0;
        virtual void visitStringTree(AST* astree)=0;
        virtual void visitNumberTree(AST* astree)=0;
};

class ASTPrintVisitor: public ASTVisitor {
    public:
        ASTPrintVisitor();
        void visitChildren(AST* astree) override;
        void visitAssignTree (AST* astree) override;
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
        void visitFunctionTree (AST* astree) override;
        void visitFunctionDeclarationTree (AST* astree) override;
        void visitWhileTree (AST* astree) override;
        void visitElseTree (AST* astree) override;
        void visitStringTree(AST* astree) override;
        void visitNumberTree(AST* astree) override;
        void visitExponentTree(AST* astree) override;

        void printIndent();
        void printer(std::string type, AST* node, std::string symbol);
        void printer(std::string type, AST* node);

    private:
        int indent;
        int lineNum;
};

class ASTDeallocationVisitor: public ASTVisitor {
    public:
        ASTDeallocationVisitor();
        void visitChildren(AST* astree) override;
        void visitAssignTree (AST* astree) override;
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
        void visitFunctionTree (AST* astree) override;
        void visitFunctionDeclarationTree (AST* astree) override;
        void visitWhileTree (AST* astree) override;
        void visitElseTree (AST* astree) override;
        void visitStringTree(AST* astree) override;
        void visitNumberTree(AST* astree) override;
        void visitExponentTree(AST* astree) override;

        void deAllocateChildren();
        void deallocate(AST* node);

        private:
            int nodenum;
};

#endif