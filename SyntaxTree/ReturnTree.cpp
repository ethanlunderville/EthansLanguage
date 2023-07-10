#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ReturnTree::ReturnTree(std::string retval) {}

ReturnTree::ReturnTree() {}

void ReturnTree::accept(ASTVisitor* v) { v->visitReturnTree(this); }