#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

EqualTree::EqualTree() : Operator() {
    this->setPrecedence(1);
}

double EqualTree::equal(double x, double y) {
    double res = x == y;
    this->setVal(res);
    return res; 
}

void EqualTree::accept(ASTVisitor* v) { v->visitEqualTree(this); }