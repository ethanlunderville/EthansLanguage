#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include <cmath>

ExponentTree::ExponentTree() : Operator() {
    precedence = 4;
}
double ExponentTree::exponent(double x, double y) {
    double res = std::pow(x,y);
    this->setVal(res);
    return res; 
}
void ExponentTree::accept(ASTVisitor* v) { v->visitExponentTree(this); }