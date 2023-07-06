/*

    File: [file]

    Description:
    [Brief description of the file's purpose and main functionality]

    Notes:
    [Additional notes, caveats, or important information]
    
*/
#pragma once
#include <vector>
#include <string>
#include <any>
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

class IfTree : public AST {
    public:
        IfTree();
        void accept(ASTVisitor* v) override;
};

class ElseTree : public AST {
    public:
        ElseTree();
        void accept(ASTVisitor* v) override;
};

class WhileTree : public AST {
    public:
        WhileTree();
        void accept(ASTVisitor* v) override;
};

class BlockTree : public AST {
    public:
        BlockTree();
        void accept(ASTVisitor* v) override;
};

class FunctionTree : public AST {
    public:
        FunctionTree();
        void accept(ASTVisitor* v) override;
};

class AssignTree : public AST {
    public:
        AssignTree();
        AssignTree(std::string identifier, int line);
        std::string getIdentifier();
        int getLine();
        void accept(ASTVisitor* v) override;
    private:
        std::string identifier;
        int line;
};

class ReturnTree : public AST {
    public:
        ReturnTree(std::string retval);
        ReturnTree();
        void accept(ASTVisitor* v) override;
    private:
        std::string name;
};

class FunctionDeclarationTree : public AST {
    public:
        FunctionDeclarationTree(std::string name);
        void accept(ASTVisitor* v) override;
    private:
        std::string name;
};

class DeclarationTree : public AST {
    public:
        DeclarationTree(std::string type, std::string name, int line);
        void accept(ASTVisitor* v) override;
        std::string getIdentifier();
        std::string getType();
        int getLine();
    private:
        std::string identifier;
        std::string type;
        int line;
};

//EVALUATABLES

//OPERANDS

class Evaluatable : public AST {
    public:
        Evaluatable();
        virtual ~Evaluatable();
        std::any getVal();
        void setVal(std::any value);
    private:
        std::any value;
};

class NumberTree : public Evaluatable {
    public:
        NumberTree(std::string number);
        double getNumber();
        void accept(ASTVisitor* v) override;
    private:
        double number;
};

class StringTree : public Evaluatable {
    public:
        StringTree(std::string sString);
        std::string getString();
        void accept(ASTVisitor* v) override;
    private:
        std::string sString;
};

class ExpressionTree : public Evaluatable {
    public:
        ExpressionTree();
        void accept(ASTVisitor* v) override;
};

class StringExpressionTree : public ExpressionTree {
    public:
        StringExpressionTree();
        void accept(ASTVisitor* v) override;
};

//OPERATORS

class Operator : public Evaluatable {
    public:
        Operator();
        virtual ~Operator();
        void setPrecendence(short precedence);
        short getPrecendence();
        void accept(ASTVisitor* v) override;
        short precedence;

    //private:
        
};

class AddTree : public Operator {
    public:
        AddTree();
        std::string add(double x, std::string y);
        std::string add(std::string x, double y);
        std::string add(std::string x, std::string y);
        double add(double x, double y);
        void accept(ASTVisitor* v) override;
};

class SubtractTree : public Operator {
    public:
        SubtractTree();
        double subtract(double x, double y);
        void accept(ASTVisitor* v) override;
};

class MultiplyTree : public Operator {
    public:
        MultiplyTree();
        double multiply(double x, double y);
        void accept(ASTVisitor* v) override;
};

class DivideTree : public Operator {
    public:
        DivideTree();
        double divide(double x, double y);
        void accept(ASTVisitor* v) override;
};

class ExponentTree : public Operator {
    public:
        ExponentTree();
        double exponent(double x, double y);
        void accept(ASTVisitor* v) override;
};

class GreaterTree : public Operator {
    public:
        GreaterTree();
        double greaterThan(double x, double y);
        void accept(ASTVisitor* v) override;
};

class LessTree : public Operator {
    public:
        LessTree();
        double lessThan(double x, double y);
        void accept(ASTVisitor* v) override;
};

class GreaterEqualTree : public Operator {
    public:
        GreaterEqualTree();
        double greaterEqual(double x, double y);
        void accept(ASTVisitor* v) override;
};

class LessEqualTree : public Operator {
    public:
        LessEqualTree();
        double lessEqual(double x, double y);
        void accept(ASTVisitor* v) override;
};

class EqualTree : public Operator {
    public:
        EqualTree();
        double equal(double x, double y);
        void accept(ASTVisitor* v) override;
};

class NotEqualTree : public Operator {
    public:
        NotEqualTree();
        double notEqual(double x, double y);
        void accept(ASTVisitor* v) override;
};

class AndTree : public Operator {
    public:
        AndTree();
        double opAnd(double x, double y);
        void accept(ASTVisitor* v) override;
};

class OrTree : public Operator {
    public:
        OrTree();
        double opOr(double x, double y);
        void accept(ASTVisitor* v) override;
};