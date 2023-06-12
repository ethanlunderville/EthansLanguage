#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

BlockTree::BlockTree() {}
void BlockTree::accept(ASTVisitor* v) { v->visitBlockTree(this); }