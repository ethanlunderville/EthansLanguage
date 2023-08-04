#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

StructDeclarationTree::StructDeclarationTree(std::string type, std::string identifier, int line) {
    this->identifier = identifier;
    this->type = type;
    this->line = line;
}

void StructDeclarationTree::accept(ASTVisitor* v) { v->visitStructDeclarationTree(this); }