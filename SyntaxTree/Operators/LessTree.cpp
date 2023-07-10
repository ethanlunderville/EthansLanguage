#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

LessTree::LessTree() : Operator() {
    this->setPrecedence(1);
}

double LessTree::lessThan(double x, double y) {
    double res = x < y;
    this->setVal(res);
    return res; 
}

void LessTree::accept(ASTVisitor* v) { v->visitLessTree(this); }