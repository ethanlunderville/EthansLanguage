#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

GreaterEqualTree::GreaterEqualTree() : Operator() {
    this->setPrecedence(1);
}

double GreaterEqualTree::greaterEqual(double x, double y) {
    double res = x >= y;
    this->setVal(res);
    return res; 
}

void GreaterEqualTree::accept(ASTVisitor* v) { v->visitGreaterEqualTree(this); }