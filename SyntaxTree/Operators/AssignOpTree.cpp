#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AssignOpTree::AssignOpTree() : Operator() {
    this->setPrecedence(0);
}

void AssignOpTree::assign(std::any* x, std::any y) {
    (*x) = y;
}

void AssignOpTree::accept(ASTVisitor* v) { v->visitAssignOpTree(this); }