#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

StructAssignTree::StructAssignTree(std::string identifier, int line) {
    this->identifier = identifier;
    this->line = line;
}

int StructAssignTree::getLine() {
    return this->line;
}

std::string StructAssignTree::getIdentifier() {
    return this->identifier;
}

void StructAssignTree::accept(ASTVisitor* v) { v->visitStructAssignTree(this); }