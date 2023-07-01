#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

DeclarationTree::DeclarationTree(std::string type,std::string identifier, int line) {
    this->type = type;
    this->identifier = identifier;
    this->line = line;
}
std::string DeclarationTree::getIdentifier() {
    return this->identifier;
}
std::string DeclarationTree::getType() {
    return this->type;
}
int DeclarationTree::getLine() {
    return this->line;
}
void DeclarationTree::accept(ASTVisitor* v) { v->visitDeclarationTree(this); }