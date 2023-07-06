#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

DivideTree::DivideTree() : Operator() {
    precedence = 3;
}
double DivideTree::divide(double x, double y) {
    double res = x / y;
    this->setVal(res);
    return res; 
}
void DivideTree::accept(ASTVisitor* v) { v->visitDivideTree(this); }