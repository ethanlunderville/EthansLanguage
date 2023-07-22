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
class Type;
class Function;
class String;
class Array;
class Number;
class Struct;

class AST {
    public:
        AST();
        virtual ~AST();
        void addChild(AST* tree);
        std::vector<AST*> getChildren();
        virtual void accept(ASTVisitor* v) = 0;
        void prependToChildren(AST* prependNode);
    private:
        std::vector<AST*> children;
};

class PrintTree : public AST {
    public:
        PrintTree();
        void accept(ASTVisitor* v) override;
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
class Evaluatable : public AST {
    public:
        Evaluatable();
        virtual ~Evaluatable();
        std::any getVal();
        void setVal(std::any value);
    private:
        std::any value;
};

class Assignable : public Evaluatable {
    public:
        virtual ~Assignable() {}

        virtual void checkStruct(Struct* type) = 0;
        virtual void checkFunction(Function* type) = 0;
        virtual void checkArray(Array* arrayType) = 0;
        virtual void checkNumber(Number* arrayType) = 0;
        virtual void checkString(String* arrayType) = 0;
        virtual void assignStruct(Struct* type) = 0;
        virtual void assignFunction(Function* type) = 0;
        virtual void assignArray(Array* arrayType) = 0;
        virtual void assignNumber(Number* arrayType) = 0;
        virtual void assignString(String* arrayType) = 0;
};

class AssignTree : public Assignable {
    public:
        AssignTree(std::string identifier, int line);
        std::string getIdentifier();
        int getLine();
        void accept(ASTVisitor* v) override;

        void checkStruct(Struct* type) override;
        void checkFunction(Function* type) override;
        void checkArray(Array* arrayType) override;
        void checkNumber(Number* arrayType) override;
        void checkString(String* arrayType) override;
        void assignStruct(Struct* type) override;
        void assignFunction(Function* type) override;
        void assignArray(Array* arrayType) override;
        void assignNumber(Number* arrayType) override;
        void assignString(String* arrayType) override;

    private:
        std::string identifier;
        int line;
};

class FunctionAssignTree : public Assignable {
    public:
        FunctionAssignTree();
        FunctionAssignTree(std::string identifier, int line);
        std::string getIdentifier();
        int getLine();
        void accept(ASTVisitor* v) override;

        void checkStruct(Struct* type) override;
        void checkFunction(Function* type) override;
        void checkArray(Array* arrayType) override;
        void checkNumber(Number* arrayType) override;
        void checkString(String* arrayType) override;
        void assignStruct(Struct* type) override;
        void assignFunction(Function* type) override;
        void assignArray(Array* arrayType) override;
        void assignNumber(Number* arrayType) override;
        void assignString(String* arrayType) override;

    private:
        std::string identifier;
        int line;
};
class ArrayAssignTree : public Assignable {
    public:
        ArrayAssignTree();
        ArrayAssignTree(std::string identifier, int line);
        std::string getIdentifier();
        int getLine();
        void accept(ASTVisitor* v) override;

        void checkStruct(Struct* type) override;
        void checkFunction(Function* type) override;
        void checkArray(Array* arrayType) override;
        void checkNumber(Number* arrayType) override;
        void checkString(String* arrayType) override;
        void assignStruct(Struct* type) override;
        void assignFunction(Function* type) override;
        void assignArray(Array* arrayType) override;
        void assignNumber(Number* arrayType) override;
        void assignString(String* arrayType) override;

    private:
        std::string identifier;
        int line;
};
class StructAssignTree : public Assignable {
    public:
        StructAssignTree();
        StructAssignTree(std::string typeName, int line);
        std::string getTypeName();
        int getLine();
        void accept(ASTVisitor* v) override;

        void checkStruct(Struct* type) override;
        void checkFunction(Function* type) override;
        void checkArray(Array* arrayType) override;
        void checkNumber(Number* arrayType) override;
        void checkString(String* arrayType) override;
        void assignStruct(Struct* type) override;
        void assignFunction(Function* type) override;
        void assignArray(Array* arrayType) override;
        void assignNumber(Number* arrayType) override;
        void assignString(String* arrayType) override;

    private:
        std::string identifier;
        int line;
        std::string typeName;
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

class ExpressionTree : public Assignable {
    public:
        ExpressionTree();

        void checkStruct(Struct* type) override;
        void checkFunction(Function* type) override;
        void checkArray(Array* arrayType) override;
        void checkNumber(Number* arrayType) override;
        void checkString(String* arrayType) override;
        void assignStruct(Struct* type) override;
        void assignFunction(Function* type) override;
        void assignArray(Array* arrayType) override;
        void assignNumber(Number* arrayType) override;
        void assignString(String* arrayType) override;
        
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