#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ReturnTree::ReturnTree() {}

void ReturnTree::accept(ASTVisitor* v) { v->visitReturnTree(this); }