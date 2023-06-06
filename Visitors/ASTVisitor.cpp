class ASTVisitor {

    void visitChildren(AST astree) {
        for (int i = 0 ; i < astree.getChildren() ; i++) {
            astree[i].accept(this);
        }
    }

}