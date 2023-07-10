#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

Evaluatable::Evaluatable() : AST() {}

Evaluatable::~Evaluatable() {}

std::any Evaluatable::getVal() {
    return this->value;
}

void Evaluatable::setVal(std::any value) {
    this->value = value;
}