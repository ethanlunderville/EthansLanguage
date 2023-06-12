#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AddTree::AddTree() {}
void AddTree::accept(ASTVisitor* v) { v->visitAddTree(this); }