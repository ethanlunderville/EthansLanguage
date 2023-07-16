#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

FunctionCallTree::FunctionCallTree(std::string identifier) : Evaluatable() {
    this->identifier = identifier;
}
std::string FunctionCallTree::getIdentifier() {
    return this->identifier;
}
void FunctionCallTree::accept(ASTVisitor* v) { v->visitFunctionCallTree(this); }