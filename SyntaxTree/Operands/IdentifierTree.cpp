#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

IdentifierTree::IdentifierTree(std::string identifier) {
    this->setIdentifier(identifier);
}

std::string IdentifierTree::getIdentifier() {
    return this->identifier;
}

void IdentifierTree::setIdentifier(std::string identifier) {
    this->identifier = identifier;
}

void IdentifierTree::accept(ASTVisitor* v) {
    v->visitIdentifierTree(this);
}