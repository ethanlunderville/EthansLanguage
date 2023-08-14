#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

StructDeclarationTree::StructDeclarationTree(std::string type, std::string identifier, int line)
 : Declarable(type, identifier, line) {}

void StructDeclarationTree::accept(ASTVisitor* v) { v->visitStructDeclarationTree(this); }