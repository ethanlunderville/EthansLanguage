#pragma once
#include <vector>
#include <string>
#include "AST.h"
class ASTVisitor;

class AST {
    public:
        AST();
        void addChild(AST* tree);
        std::vector<AST*> getChildren();
        virtual void accept(ASTVisitor* v) = 0;
        virtual ~AST();
    private:
        std::vector<AST*> children;
};

class ProgramTree : public AST {
    public:
        ProgramTree();
        void accept(ASTVisitor* v) override;
};

class DeclarationTree : public AST {
    public:
        DeclarationTree(std::string name, std::string val);
        void accept(ASTVisitor* v);
    private:
        std::string name;
};

class IfTree : public AST {
    public:
        IfTree();
        void accept(ASTVisitor* v);
};

class ElseTree : public AST {
    public:
        ElseTree();
        void accept(ASTVisitor* v);
};

class WhileTree : public AST {
    public:
        WhileTree();
        void accept(ASTVisitor* v);
};
class ReturnTree : public AST {
    public:
        ReturnTree(std::string retval);
        ReturnTree();
        void accept(ASTVisitor* v);
    private:
        std::string name;
};
class ExpressionTree : public AST {
    public:
        ExpressionTree();
        void accept(ASTVisitor* v);
};

class BlockTree : public AST {
    public:
        BlockTree();
        void accept(ASTVisitor* v);
};

class FunctionTree : public AST {
    public:
        FunctionTree();
        void accept(ASTVisitor* v);
};

class FunctionDeclarationTree : public AST {
    public:
        FunctionDeclarationTree(std::string name);
        void accept(ASTVisitor* v);
    private:
        std::string name;
};

class AssignTree : public AST {
    public:
        AssignTree();
        AssignTree(std::string name);
        AssignTree(std::string name, std::string val);
        void accept(ASTVisitor* v);
    private:
        std::string name;
        std::string val;
};

//OPERANDS

class NumberTree : public AST {
    public:
        NumberTree(std::string number);
        void accept(ASTVisitor* v);
        double getNumber();
    private:
        double number;
};

class StringTree : public AST {
    public:
        StringTree(std::string sString);
        void accept(ASTVisitor* v);
        std::string getString();
    private:
        std::string sString;
};

//OPERATORS

class Operator {
    public:
        Operator();
        virtual ~Operator();
        short getPrecendence();
        short precedence; 
};

class AddTree : public AST, public Operator {
    public:
        AddTree();
        void accept(ASTVisitor* v) override;
};

class SubtractTree : public AST, public Operator {
    public:
        SubtractTree();
        void accept(ASTVisitor* v) override;
};

class MultiplyTree : public AST, public Operator {
    public:
        MultiplyTree();
        void accept(ASTVisitor* v) override;
};

class DivideTree : public AST, public Operator {
    public:
        DivideTree();
        void accept(ASTVisitor* v) override;
};

class ExponentTree : public AST, public Operator {
    public:
        ExponentTree();
        void accept(ASTVisitor* v) override;
};