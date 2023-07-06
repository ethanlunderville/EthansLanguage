#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

MultiplyTree::MultiplyTree() : Operator() {
    precedence = 3;
}

double MultiplyTree::multiply(double x, double y) {
    double res = x * y;
    this->setVal(res);
    return res; 
}

void MultiplyTree::accept(ASTVisitor* v) { v->visitMultiplyTree(this); }