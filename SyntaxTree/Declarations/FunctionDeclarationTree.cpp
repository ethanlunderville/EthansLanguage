#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

FunctionDeclarationTree::FunctionDeclarationTree(std::string type, std::string identifier, int line) {
    this->identifier = identifier;
    this->type = type;
    this->line = line;
}

void FunctionDeclarationTree::accept(ASTVisitor* v) { v->visitFunctionDeclarationTree(this); }