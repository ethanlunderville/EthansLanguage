#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
StringTree::StringTree(std::string sString){
    this->sString = sString;
}

std::string StringTree::getString() {
    return this->sString;
}

void StringTree::accept(ASTVisitor* v) { v->visitStringTree(this); }