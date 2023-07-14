#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

DivideTree::DivideTree() : Operator() {
    this->setPrecedence(3);
}

std::any DivideTree::divide(std::any x, std::any y) {
    double res = std::any_cast<double>(x) / std::any_cast<double>(y);
    this->setVal(res);
    return res; 
}

void DivideTree::accept(ASTVisitor* v) { v->visitDivideTree(this); }