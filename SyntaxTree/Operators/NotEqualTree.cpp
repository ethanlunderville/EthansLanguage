#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

NotEqualTree::NotEqualTree() : Operator() {
    this->setPrecedence(1);
}

double NotEqualTree::notEqual(double x, double y) {
    double res = x != y;
    this->setVal(res);
    return res; 
}

void NotEqualTree::accept(ASTVisitor* v) { v->visitNotEqualTree(this); }