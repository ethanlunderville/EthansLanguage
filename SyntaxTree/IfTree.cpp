#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

IfTree::IfTree() {}
void IfTree::accept(ASTVisitor* v) { v->visitIfTree(this); }