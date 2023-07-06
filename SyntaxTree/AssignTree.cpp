#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AssignTree::AssignTree(std::string identifier, int line) {
    this->identifier = identifier;
    this->line = line;
}

int AssignTree::getLine() {
    return this->line;
}

std::string AssignTree::getIdentifier() {
    return this->identifier;
}
void AssignTree::accept(ASTVisitor* v) { v->visitAssignTree(this); }