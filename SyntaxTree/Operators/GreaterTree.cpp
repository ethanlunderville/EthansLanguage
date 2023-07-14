#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

GreaterTree::GreaterTree() : Operator() {
    this->setPrecedence(1);
}

std::any GreaterTree::greaterThan(std::any x, std::any y) {
    double res = std::any_cast<double>(x) > std::any_cast<double>(y);
    this->setVal(res);
    return res; 
}

void GreaterTree::accept(ASTVisitor* v) { v->visitGreaterTree(this); }