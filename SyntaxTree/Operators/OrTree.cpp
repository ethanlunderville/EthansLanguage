#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

OrTree::OrTree() {
    precedence = 0;
}
void OrTree::accept(ASTVisitor* v) { v->visitOrTree(this); }