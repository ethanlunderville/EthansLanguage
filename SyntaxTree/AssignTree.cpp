#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AssignTree::AssignTree(std::string name, std::string value) {
    this->name = name;
}
AssignTree::AssignTree(std::string name) {
    this->name = name;
}
void AssignTree::accept(ASTVisitor* v) { v->visitAssignTree(this); }