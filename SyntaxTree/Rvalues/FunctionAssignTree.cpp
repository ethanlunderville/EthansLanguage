#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

FunctionAssignTree::FunctionAssignTree(std::string identifier, int line) {
    this->identifier = identifier;
    this->line = line;
}

int FunctionAssignTree::getLine() {
    return this->line;
}

std::string FunctionAssignTree::getIdentifier() {
    return this->identifier;
}

void FunctionAssignTree::accept(ASTVisitor* v) { v->visitFunctionAssignTree(this); }