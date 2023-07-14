#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

MultiplyTree::MultiplyTree() : Operator() {
    this->setPrecedence(3);
}

std::any MultiplyTree::multiply(std::any x, std::any y) {
    double res = std::any_cast<double>(x) * std::any_cast<double>(y);
    this->setVal(res);
    return res; 
}

void MultiplyTree::accept(ASTVisitor* v) { v->visitMultiplyTree(this); }