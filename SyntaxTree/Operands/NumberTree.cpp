#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

NumberTree::NumberTree(std::string number) : Evaluatable() {
    this->number = std::stod(number);
    this->setVal(this->number);
}

double NumberTree::getNumber() {
    return this->number;
}

void NumberTree::accept(ASTVisitor* v) { v->visitNumberTree(this); }