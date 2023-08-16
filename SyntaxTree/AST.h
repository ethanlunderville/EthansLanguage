/*

    Description: Contains definitions of AST node classes.
    
*/

#pragma once
#include <vector>
#include <string>
#include <any>
#include <map>

class ASTVisitor;
class Type;
class Function;
class String;
class Array;
class Number;
class Struct;
class ASTChecker;

class AST {
    public:
        AST();
        virtual ~AST();
        void addChild(AST* tree);
        std::vector<AST*>& getChildren();
        virtual void accept(ASTVisitor* v) = 0;
        void prependToChildren(AST* prependNode);
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


class RegexSectionTree : public AST {
    public:
        RegexSectionTree(std::string& regex);
        std::string& getRegex();
        void accept(ASTVisitor* v) override;
    private:
        std::string regex;
};

//AssignTrees
class Evaluatable : public AST {
    public:
        Evaluatable();
        virtual ~Evaluatable();
        virtual std::any getVal();
        virtual void setVal(std::any value);
        virtual void setIsLVal(bool isLval);
        virtual bool getIsLVal();
    private:
        bool LVal;
        std::any value;
};

class ReturnTree : public AST {
    public:
        ReturnTree();
        void accept(ASTVisitor* v) override;
    private:
        std::string name;
};

class Assignable : public Evaluatable {
    /*COMMENTED METHODS WILL BE IMPLEMENTED WHEN COMPLEX TYPES CAN CONTAIN OTHER COMPLEX TYPES*/
    public:
        Assignable();
        virtual ~Assignable();
        virtual Type* getType();
        virtual void setType(Type* type);
        virtual int getLine();
        virtual void setLine(int line);
        virtual void setCheckerReference(ASTChecker* checkerReference); 
        virtual ASTChecker* getCheckerReference();
    private:
        int line;
        ASTChecker* checkerReference;
        Type* type;
};

class FunctionAssignTree : public Assignable {
    public:
        FunctionAssignTree();
        FunctionAssignTree(std::string identifier, int line);
        std::string getIdentifier();
        void accept(ASTVisitor* v) override;
    private:
        std::string identifier;
};
class ArrayAssignTree : public Assignable {
    public:
        ArrayAssignTree();
        ArrayAssignTree(std::string identifier, int line);
        std::string getIdentifier();
        void accept(ASTVisitor* v) override;
    private:
        std::string identifier;
};
class StructAssignTree : public Assignable {
    public:
        StructAssignTree();
        StructAssignTree(std::string typeName, int line);
        std::string getTypeName();
        void accept(ASTVisitor* v) override;
    private:
        std::string identifier;
        std::string typeName;
};

class Declarable : public AST {
    public:
        Declarable(std::string type, std::string identifier, int line);
        virtual ~Declarable();
        virtual std::string getIdentifier();
        virtual void setIdentifier(std::string& identifier);
        virtual std::string getType();
        virtual void setType(std::string& identifier);
        virtual int getLine();
    private:
        std::string identifier;
        std::string type;
        int line;
};

class DeclarationTree : public Declarable {
    public:
        DeclarationTree(std::string type, std::string identifier, int line);
        void accept(ASTVisitor* v) override;
};

class FunctionDeclarationTree : public Declarable {
    public:
        FunctionDeclarationTree(std::string type, std::string identifier, int line);
        void accept(ASTVisitor* v) override;
};

class ArrayDeclarationTree : public Declarable {
    public:
        ArrayDeclarationTree(std::string type, std::string identifier, int line);
        void accept(ASTVisitor* v) override;
};

class StructDeclarationTree : public Declarable {
    public:
        StructDeclarationTree(std::string type, std::string identifier, int line);
        void accept(ASTVisitor* v) override;
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

class Identifiable : public Evaluatable {
    public:
        ~Identifiable();
        std::string getIdentifier();
        void setIdentifier(const std::string& identifier);
    private:
        std::string identifier;
};

class ArrayAccessTree : public Identifiable {
    public:
        ArrayAccessTree(std::string identifier);
        void accept(ASTVisitor* v) override;
};

class FunctionCallTree : public Identifiable {
    public:
        FunctionCallTree(std::string identifier);
        void accept(ASTVisitor* v) override;

        bool returned;
};

class IdentifierTree : public Identifiable {
    public:
        IdentifierTree(std::string identifier);
        void accept(ASTVisitor* v) override;
};

class ExpressionTree : public Assignable {
    public:
        ExpressionTree(int line);
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
        void assign(std::any* x, std::any y);
        void accept(ASTVisitor* v) override;
};