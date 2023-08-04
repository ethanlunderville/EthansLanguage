#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ArrayDeclarationTree::ArrayDeclarationTree(std::string type, std::string identifier, int line) {
    this->identifier = identifier;
    this->type = type;
    this->line = line;
}
void ArrayDeclarationTree::accept(ASTVisitor* v) { v->visitArrayDeclarationTree(this); }