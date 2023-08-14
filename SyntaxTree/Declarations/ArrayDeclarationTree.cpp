#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ArrayDeclarationTree::ArrayDeclarationTree(std::string type, std::string identifier, int line)
 : Declarable(type, identifier, line){}
void ArrayDeclarationTree::accept(ASTVisitor* v) { v->visitArrayDeclarationTree(this); }