#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

GreaterEqualTree::GreaterEqualTree() : Operator() {
    this->setPrecedence(1);
}

std::any GreaterEqualTree::greaterEqual(std::any x, std::any y) {
    double res = std::any_cast<double>(x) >= std::any_cast<double>(y);
    this->setVal(res);
    return res; 
}

void GreaterEqualTree::accept(ASTVisitor* v) { v->visitGreaterEqualTree(this); }