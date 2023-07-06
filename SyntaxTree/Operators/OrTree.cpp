#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

OrTree::OrTree() : Operator() {
    precedence = 0;
}
double OrTree::opOr(double x, double y) {
    double res = x || y;
    this->setVal(res);
    return res; 
}
void OrTree::accept(ASTVisitor* v) { v->visitOrTree(this); }