#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AndTree::AndTree() : Operator() {
    this->setPrecedence(0);
}

std::any AndTree::opAnd(std::any x, std::any y) {
    double res = std::any_cast<double>(x) && std::any_cast<double>(y);
    this->setVal(res);
    return res; 
}

void AndTree::accept(ASTVisitor* v) { v->visitAndTree(this); }