#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AssignTree::AssignTree(std::string identifier, std::string value) {
    this->identifier = identifier;
}
AssignTree::AssignTree(std::string identifier) {
    this->identifier = identifier;
}
std::string AssignTree::getIdentifier() {
    return this->identifier;
}
void AssignTree::accept(ASTVisitor* v) { v->visitAssignTree(this); }