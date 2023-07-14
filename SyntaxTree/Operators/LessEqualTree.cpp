#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

LessEqualTree::LessEqualTree() : Operator() {
    this->setPrecedence(1);
}

std::any LessEqualTree::lessEqual(std::any x, std::any y) {
    double res = std::any_cast<double>(x) <= std::any_cast<double>(y);
    this->setVal(res);
    return res; 
}

void LessEqualTree::accept(ASTVisitor* v) { v->visitLessEqualTree(this); }