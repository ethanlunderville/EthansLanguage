#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

Evaluatable::Evaluatable() : AST() , LVal(false) {}

Evaluatable::~Evaluatable() {}

std::any Evaluatable::getVal() {
    return this->value;
}

void Evaluatable::setIsLVal(bool isLVal){
    this->LVal = isLVal;
}

bool Evaluatable::getIsLVal(){
    return this->LVal;
}

void Evaluatable::setVal(std::any value) {
    this->value = value;
}