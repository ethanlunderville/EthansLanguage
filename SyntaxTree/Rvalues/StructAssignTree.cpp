#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

StructAssignTree::StructAssignTree(std::string typeName ,int line) {
    this->typeName = typeName;
    this->setLine(line);
}
std::string StructAssignTree::getTypeName() {
    return this->typeName;
}

void StructAssignTree::accept(ASTVisitor* v) { v->visitStructAssignTree(this); }