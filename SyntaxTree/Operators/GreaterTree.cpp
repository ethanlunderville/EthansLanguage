#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

GreaterTree::GreaterTree() {
    precedence = 1;
}
void GreaterTree::accept(ASTVisitor* v) { v->visitGreaterTree(this); }