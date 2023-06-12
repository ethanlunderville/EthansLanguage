#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

MultiplyTree::MultiplyTree() {}
void MultiplyTree::accept(ASTVisitor* v) { v->visitMultiplyTree(this); }