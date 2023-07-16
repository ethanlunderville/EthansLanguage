#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

FunctionDeclarationTree::FunctionDeclarationTree(std::string type, std::string identifier, int line) {
    this->identifier = identifier;
    this->type = type;
    this->line = line;
}

std::string FunctionDeclarationTree::getIdentifier() {
    return this->identifier;
}

std::string FunctionDeclarationTree::getType() {
    return this->type;
}

int FunctionDeclarationTree::getLine() {
    return this->line;
}

void FunctionDeclarationTree::accept(ASTVisitor* v) { v->visitFunctionDeclarationTree(this); }