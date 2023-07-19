#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ArrayAssignTree::ArrayAssignTree(std::string identifier, int line) {
    this->identifier = identifier;
    this->line = line;
}

int ArrayAssignTree::getLine() {
    return this->line;
}

std::string ArrayAssignTree::getIdentifier() {
    return this->identifier;
}

void ArrayAssignTree::accept(ASTVisitor* v) { v->visitArrayAssignTree(this); }