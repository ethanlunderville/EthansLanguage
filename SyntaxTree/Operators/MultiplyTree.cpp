#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

MultiplyTree::MultiplyTree() {
    precedence = 2;
}
void MultiplyTree::accept(ASTVisitor* v) { v->visitMultiplyTree(this); }