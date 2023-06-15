#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
StringTree::StringTree(std::string sString){
    this->sString = sString;
}
void StringTree::accept(ASTVisitor* v) { v->visitStringTree(this); }