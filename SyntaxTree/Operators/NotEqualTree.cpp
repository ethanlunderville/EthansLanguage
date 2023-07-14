#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

NotEqualTree::NotEqualTree() : Operator() {
    this->setPrecedence(1);
}

std::any NotEqualTree::notEqual(std::any x, std::any y) {
    double res = std::any_cast<double>(x) != std::any_cast<double>(y);
    this->setVal(res);
    return res; 
}

void NotEqualTree::accept(ASTVisitor* v) { v->visitNotEqualTree(this); }