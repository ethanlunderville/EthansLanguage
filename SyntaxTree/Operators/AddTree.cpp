#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AddTree::AddTree() {
    precedence = 2;
}
void AddTree::accept(ASTVisitor* v) { v->visitAddTree(this); }