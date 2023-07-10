#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AndTree::AndTree() : Operator() {
    this->setPrecedence(0);
}

double AndTree::opAnd(double x, double y) {
    double res = x && y;
    this->setVal(res);
    return res; 
}

void AndTree::accept(ASTVisitor* v) { v->visitAndTree(this); }