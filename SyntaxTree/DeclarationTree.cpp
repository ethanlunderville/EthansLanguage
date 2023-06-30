#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

DeclarationTree::DeclarationTree(std::string identifier) {
    this->identifier = identifier;
}
std::string DeclarationTree::getIdentifier() {
    return this->identifier;
}
void DeclarationTree::accept(ASTVisitor* v) { v->visitDeclarationTree(this); }