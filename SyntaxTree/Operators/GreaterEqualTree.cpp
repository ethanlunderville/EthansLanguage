#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

GreaterEqualTree::GreaterEqualTree() {
    precedence = 0;
}
void GreaterEqualTree::accept(ASTVisitor* v) { v->visitGreaterEqualTree(this); }