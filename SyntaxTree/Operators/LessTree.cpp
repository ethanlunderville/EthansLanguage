#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

LessTree::LessTree() : Operator() {
    this->setPrecedence(1);
}

std::any LessTree::lessThan(std::any x, std::any y) {
    double res = std::any_cast<double>(x) < std::any_cast<double>(y);
    this->setVal(res);
    return res; 
}

void LessTree::accept(ASTVisitor* v) { v->visitLessTree(this); }