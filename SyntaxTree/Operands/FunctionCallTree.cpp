#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

FunctionCallTree::FunctionCallTree(std::string identifier){
    this->setIdentifier(identifier);
}
void FunctionCallTree::accept(ASTVisitor* v) { v->visitFunctionCallTree(this); }