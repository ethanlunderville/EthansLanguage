#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include "Keywords.h"

DeclarationTree::DeclarationTree(std::string type,std::string identifier, int line) {
    this->type = type;
    this->identifier = identifier;
    this->line = line;
}

void DeclarationTree::accept(ASTVisitor* v) { v->visitDeclarationTree(this); }