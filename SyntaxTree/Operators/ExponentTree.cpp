#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include <cmath>

ExponentTree::ExponentTree() : Operator() {
    this->setPrecedence(4);
}

std::any ExponentTree::exponent(std::any x, std::any y) {
    double res = std::pow(std::any_cast<double>(x),std::any_cast<double>(y));
    this->setVal(res);
    return res; 
}

void ExponentTree::accept(ASTVisitor* v) { v->visitExponentTree(this); }