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
        virtual ~AST();
        void addChild(AST* tree);
        std::vector<AST*> getChildren();
        virtual void accept(ASTVisitor* v) = 0; 
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

//AssignTrees

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

class FunctionAssignTree : public AST {
    public:
        FunctionAssignTree();
        FunctionAssignTree(std::string identifier, int line);
        std::string getIdentifier();
        int getLine();
        void accept(ASTVisitor* v) override;
    private:
        std::string identifier;
        int line;
};
class ArrayAssignTree : public AST {
    public:
        ArrayAssignTree();
        ArrayAssignTree(std::string identifier, int line);
        std::string getIdentifier();
        int getLine();
        void accept(ASTVisitor* v) override;
    private:
        std::string identifier;
        int line;
};
class StructAssignTree : public AST {
    public:
        StructAssignTree();
        StructAssignTree(std::string identifier, int line);
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

class DeclarationTree : public AST {
    public:
        DeclarationTree(std::string type, std::string identifier, int line);
        void accept(ASTVisitor* v) override;
        std::string getIdentifier();
        std::string getType();
        int getLine();
    private:
        std::string identifier;
        std::string type;
        int line;
};

class FunctionDeclarationTree : public AST {
    public:
        FunctionDeclarationTree(std::string type, std::string identifier, int line);
        void accept(ASTVisitor* v) override;
        std::string getIdentifier();
        std::string getType();
        int getLine();
    private:
        std::string identifier;
        std::string type;
        int line;
};

class ArrayDeclarationTree : public AST {
    public:
        ArrayDeclarationTree(std::string type, std::string identifier, int line);
        void accept(ASTVisitor* v) override;
        std::string getIdentifier();
        std::string getType();
        int getLine();
    private:
        std::string identifier;
        std::string type;
        int line;
};

class StructDeclarationTree : public AST {
    public:
        StructDeclarationTree(std::string type, std::string identifier, int line);
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


class FunctionCallTree : public Evaluatable {
    public:
        FunctionCallTree(std::string identifier);
        void accept(ASTVisitor* v) override;
        std::string getIdentifier();
    private:
        std::string identifier;
};

class IdentifierTree : public Evaluatable {
    public:
        IdentifierTree(std::string identifier);
        IdentifierTree(std::string identifier, AST* subScript);
        void accept(ASTVisitor* v) override;
        std::string getIdentifier();
        void setIdentifier(std::string identifier);

    private:
        std::string identifier;
        AST* subScript;
};

class ExpressionTree : public Evaluatable {
    public:
        ExpressionTree();
        void accept(ASTVisitor* v) override;
};

class StringExpressionTree : public Evaluatable {
    public:
        StringExpressionTree();
        void accept(ASTVisitor* v) override;
};

//OPERATORS

class Operator : public Evaluatable {
    public:
        Operator();
        virtual ~Operator();
        void setPrecedence(short precedence);
        short getPrecendence();
        void accept(ASTVisitor* v) override;
    private:
        short precedence;
};

class AddTree : public Operator {
    public:
        AddTree();
        std::string add(double x, std::string y);
        std::string add(std::string x, double y);
        std::string add(std::string x, std::string y);
        double add(double x, double y);
        std::any add(std::any operand1, std::any operand2);
        void accept(ASTVisitor* v) override;
};

class SubtractTree : public Operator {
    public:
        SubtractTree();
        std::any subtract(std::any x, std::any y);
        void accept(ASTVisitor* v) override;
};

class MultiplyTree : public Operator {
    public:
        MultiplyTree();
        std::any multiply(std::any x, std::any y);
        void accept(ASTVisitor* v) override;
};

class DivideTree : public Operator {
    public:
        DivideTree();
        std::any divide(std::any x, std::any y);
        void accept(ASTVisitor* v) override;
};

class ExponentTree : public Operator {
    public:
        ExponentTree();
        std::any exponent(std::any x, std::any y);
        void accept(ASTVisitor* v) override;
};

class GreaterTree : public Operator {
    public:
        GreaterTree();
        std::any greaterThan(std::any x, std::any y);
        void accept(ASTVisitor* v) override;
};

class LessTree : public Operator {
    public:
        LessTree();
        std::any lessThan(std::any x, std::any y);
        void accept(ASTVisitor* v) override;
};

class GreaterEqualTree : public Operator {
    public:
        GreaterEqualTree();
        std::any greaterEqual(std::any x, std::any y);
        void accept(ASTVisitor* v) override;
};

class LessEqualTree : public Operator {
    public:
        LessEqualTree();
        std::any lessEqual(std::any x, std::any y);
        void accept(ASTVisitor* v) override;
};

class EqualTree : public Operator {
    public:
        EqualTree();
        std::any equal(std::any x, std::any y);
        void accept(ASTVisitor* v) override;
};

class NotEqualTree : public Operator {
    public:
        NotEqualTree();
        std::any notEqual(std::any x, std::any y);
        void accept(ASTVisitor* v) override;
};

class AndTree : public Operator {
    public:
        AndTree();
        std::any opAnd(std::any x, std::any y);
        void accept(ASTVisitor* v) override;
};

class OrTree : public Operator {
    public:
        OrTree();
        std::any opOr(std::any x, std::any y);
        void accept(ASTVisitor* v) override;
};

class ArrowOpTree : public Operator {
    public:
        ArrowOpTree();
        std::any getOp(std::any x, std::any y);
        void accept(ASTVisitor* v) override;
};

class AssignOpTree : public Operator {
    public:
        AssignOpTree();
        std::any assign(std::any x, std::any y);
        void accept(ASTVisitor* v) override;
};