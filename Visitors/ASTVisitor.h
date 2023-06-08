
class ASTVisitor {
    public:
        void visitChildren(AST astree);
        void visitProgramTree(ProgramTree* pTree);
};

class ASTPrintVisitor: public ASTVisitor {
    public:
        void visitChildren(AST astree);
        void printNode(AST astree);
        void visitProgramTree(ProgramTree* pTree);
        //DEFINE PRINT FUNCTION FOR EACH TYPE HERE
};