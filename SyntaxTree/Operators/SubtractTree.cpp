#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

SubtractTree::SubtractTree() : Operator() {
    this->setPrecedence(2);
}

std::any SubtractTree::subtract(std::any x, std::any y) {
    double res = std::any_cast<double>(x) - std::any_cast<double>(y);
    this->setVal(res);
    return res; 
}

void SubtractTree::accept(ASTVisitor* v) { v->visitSubtractTree(this); }