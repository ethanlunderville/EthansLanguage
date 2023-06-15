#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
ExponentTree::ExponentTree(std::string number){
    this->number = std::stod(number);
}
void ExponentTree::accept(ASTVisitor* v) { v->visitExponentTree(this); }