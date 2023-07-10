#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

FunctionDeclarationTree::FunctionDeclarationTree(std::string name) {
    this->name = name;
}

void FunctionDeclarationTree::accept(ASTVisitor* v) { v->visitFunctionDeclarationTree(this); }