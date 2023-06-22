#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

EqualTree::EqualTree() {
    precedence = 0;
}
void EqualTree::accept(ASTVisitor* v) { v->visitEqualTree(this); }