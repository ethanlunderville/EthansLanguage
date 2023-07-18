#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ArrayDeclarationTree::ArrayDeclarationTree(std::string type, std::string identifier, int line) {
    this->identifier = identifier;
    this->type = type;
    this->line = line;
}

std::string ArrayDeclarationTree::getIdentifier() {
    return this->identifier;
}

std::string ArrayDeclarationTree::getType() {
    return this->type;
}

int ArrayDeclarationTree::getLine() {
    return this->line;
}

void ArrayDeclarationTree::accept(ASTVisitor* v) { v->visitArrayDeclarationTree(this); }