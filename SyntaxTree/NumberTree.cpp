#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
NumberTree::NumberTree(std::string number){
    this->number = std::stod(number);
}
void NumberTree::accept(ASTVisitor* v) { v->visitNumberTree(this); }