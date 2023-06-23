#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ExponentTree::ExponentTree(){
    precedence = 4;
}
void ExponentTree::accept(ASTVisitor* v) { v->visitExponentTree(this); }