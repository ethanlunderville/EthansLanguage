#include <vector>
#include <string>
class ASTVisitor;

class AST {
    public:
        AST();
        void addChild(AST* tree);
        std::vector<AST*> getChildren();
        virtual void accept(ASTVisitor* v) = 0;
    private:
        std::vector<AST*> children;
};

class ProgramTree : public AST {
    public:
        ProgramTree();
        void accept(ASTVisitor* v);
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

//OPERATORS

class AddTree : public AST {
    public:
        AddTree();
        void accept(ASTVisitor* v);
};

class SubtractTree : public AST {
    public:
        SubtractTree();
        void accept(ASTVisitor* v);
};

class MultiplyTree : public AST {
    public:
        MultiplyTree();
        void accept(ASTVisitor* v);
};

class DivideTree : public AST {
    public:
        DivideTree();
        void accept(ASTVisitor* v);
};