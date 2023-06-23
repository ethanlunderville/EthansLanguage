#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AndTree::AndTree() {
    precedence = 0;
}
void AndTree::accept(ASTVisitor* v) { v->visitAndTree(this); }