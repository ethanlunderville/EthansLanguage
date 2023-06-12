#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

DivideTree::DivideTree() {}
void DivideTree::accept(ASTVisitor* v) { v->visitDivideTree(this); }