#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

IdentifierTree::IdentifierTree(std::string identifier, AST* subScript) {
    this->setIdentifier(identifier);
    this->setSubscript(subScript);
}
IdentifierTree::IdentifierTree(std::string identifier) {
    this->setIdentifier(identifier);
    this->setSubscript(nullptr);
}

std::string IdentifierTree::getIdentifier() {
    return this->identifier;
}

void IdentifierTree::setIdentifier(std::string identifier) {
    this->identifier = identifier;
}

AST* IdentifierTree::getSubscript() {
    return this->subScript;
}

void IdentifierTree::setSubscript(AST* subScript) {
    this->subScript = subScript;
}

void IdentifierTree::accept(ASTVisitor* v) {
    v->visitIdentifierTree(this);
}