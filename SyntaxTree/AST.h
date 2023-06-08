class ASTVisitor;

class AST {
    public:
        AST();
        void addChild(AST* tree);
        std::vector<AST*> getChildren();
        void* accept(ASTVisitor v);
    private:
        std::vector<AST*> children;
};

class ProgramTree : public AST {
    public:
        ProgramTree();
        void accept(ASTVisitor v);
};

class DeclarationTree : public AST {
    public:
        ProgramTree();
        void accept(ASTVisitor v);
};

class IfTree : public AST {
    public:
        ProgramTree();
        void accept(ASTVisitor v);
};
class WhileTree : public AST {
    public:
        ProgramTree();
        void accept(ASTVisitor v);
};
class ReturnTree : public AST {
    public:
        ProgramTree();
        void accept(ASTVisitor v);
};
class IdentifierTree : public AST {
    public:
        ProgramTree();
        void accept(ASTVisitor v);
};

class ExpressionTree : public AST {
    public:
        ProgramTree();
        void accept(ASTVisitor v);
};

class BlockTree : public AST {
    public:
        ProgramTree();
        void accept(ASTVisitor v);
};


