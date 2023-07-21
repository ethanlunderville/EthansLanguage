#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

PrintTree::PrintTree() {}

void PrintTree::accept(ASTVisitor* v) { v->visitPrintTree(this); }