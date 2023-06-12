#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

DeclarationTree::DeclarationTree(std::string name, std::string val) {
    this->name = name;
}
void DeclarationTree::accept(ASTVisitor* v) { v->visitDeclarationTree(this); }