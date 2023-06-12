#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

SubtractTree::SubtractTree() {}
void SubtractTree::accept(ASTVisitor* v) { v->visitSubtractTree(this); }