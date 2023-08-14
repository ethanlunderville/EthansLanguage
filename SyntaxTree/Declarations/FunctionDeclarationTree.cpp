#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

FunctionDeclarationTree::FunctionDeclarationTree(std::string type, std::string identifier, int line)
 : Declarable(type, identifier, line) {}

void FunctionDeclarationTree::accept(ASTVisitor* v) { v->visitFunctionDeclarationTree(this); }