#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

DivideTree::DivideTree() {
    precedence = 3;
}
void DivideTree::accept(ASTVisitor* v) { v->visitDivideTree(this); }