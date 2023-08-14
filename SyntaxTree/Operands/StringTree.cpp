#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include <algorithm>

StringTree::StringTree(std::string sString) : Evaluatable() {
    char quote = '\"'; 
    sString.erase(std::remove(sString.begin(), sString.end(), quote), sString.end());
    this->sString = sString.substr(0, sString.size() - 1);
    this->setVal(this->sString);
}

std::string StringTree::getString() {
    return this->sString;
}

void StringTree::accept(ASTVisitor* v) { v->visitStringTree(this); }