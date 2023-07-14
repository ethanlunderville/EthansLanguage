#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

EqualTree::EqualTree() : Operator() {
    this->setPrecedence(1);
}

std::any EqualTree::equal(std::any x, std::any y) {
    double res = std::any_cast<double>(x) == std::any_cast<double>(y);
    this->setVal(res);
    return res; 
}

void EqualTree::accept(ASTVisitor* v) { v->visitEqualTree(this); }