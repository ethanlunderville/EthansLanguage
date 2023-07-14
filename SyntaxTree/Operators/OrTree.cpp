#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

OrTree::OrTree() : Operator() {
    this->setPrecedence(0);
}

std::any OrTree::opOr(std::any x, std::any y) {
    double res = std::any_cast<double>(x) || std::any_cast<double>(y);
    this->setVal(res);
    return res; 
}

void OrTree::accept(ASTVisitor* v) { v->visitOrTree(this); }