#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

FunctionTree::FunctionTree() {}

void FunctionTree::accept(ASTVisitor* v) { v->visitFunctionTree(this); }