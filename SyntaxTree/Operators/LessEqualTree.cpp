#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

LessEqualTree::LessEqualTree() : Operator() {
    this->setPrecedence(1);
}

double LessEqualTree::lessEqual(double x, double y) {
    double res = x <= y;
    this->setVal(res);
    return res; 
}

void LessEqualTree::accept(ASTVisitor* v) { v->visitLessEqualTree(this); }