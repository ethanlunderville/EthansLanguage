#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

IdentifierTree::IdentifierTree(std::string identifier) {
    this->setIdentifier(identifier);
}

void IdentifierTree::accept(ASTVisitor* v) {
    v->visitIdentifierTree(this);
}