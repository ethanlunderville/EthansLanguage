class AST {

    public:

        AST() {
            
        }

        std::vector<AST> getChildren() {
            return children;
        }

        void* accept(ASTVisitor v);

    private:

        std::vector<AST> children;
        
}