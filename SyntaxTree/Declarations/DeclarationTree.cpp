#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

DeclarationTree::DeclarationTree(std::string type,std::string identifier, int line)
 : Declarable(type, identifier, line) {}

void DeclarationTree::accept(ASTVisitor* v) { v->visitDeclarationTree(this); }