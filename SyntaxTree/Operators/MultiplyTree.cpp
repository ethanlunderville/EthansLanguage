#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

MultiplyTree::MultiplyTree() {
    precedence = 3;
}
void MultiplyTree::accept(ASTVisitor* v) { v->visitMultiplyTree(this); }