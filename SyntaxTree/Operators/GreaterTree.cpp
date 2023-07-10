#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

GreaterTree::GreaterTree() : Operator() {
    this->setPrecedence(1);
}

double GreaterTree::greaterThan(double x, double y) {
    double res = x > y;
    this->setVal(res);
    return res; 
}

void GreaterTree::accept(ASTVisitor* v) { v->visitGreaterTree(this); }