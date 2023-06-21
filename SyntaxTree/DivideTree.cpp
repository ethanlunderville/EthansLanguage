#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

DivideTree::DivideTree() {
    precedence = 2;
}
void DivideTree::accept(ASTVisitor* v) { v->visitDivideTree(this); }