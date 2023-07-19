#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AssignOpTree::AssignOpTree() : Operator() {
    this->setPrecedence(0);
}

std::any AssignOpTree::assign(std::any x, std::any y) {
    return 1; 
}

void AssignOpTree::accept(ASTVisitor* v) { v->visitAssignOpTree(this); }