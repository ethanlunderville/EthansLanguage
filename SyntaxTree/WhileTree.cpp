#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

WhileTree::WhileTree() {}

void WhileTree::accept(ASTVisitor* v) { v->visitWhileTree(this); }