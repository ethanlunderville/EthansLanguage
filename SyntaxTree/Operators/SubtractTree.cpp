#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

SubtractTree::SubtractTree() : Operator() {
    this->setPrecedence(2);
}

double SubtractTree::subtract(double x, double y) {
    double res = x - y;
    this->setVal(res);
    return res; 
}

void SubtractTree::accept(ASTVisitor* v) { v->visitSubtractTree(this); }