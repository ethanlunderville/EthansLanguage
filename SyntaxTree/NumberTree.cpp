#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

NumberTree::NumberTree(std::string number){
    this->number = std::stod(number);
}
double NumberTree::getNumber() {
    return this->number;
}
void NumberTree::accept(ASTVisitor* v) { v->visitNumberTree(this); }