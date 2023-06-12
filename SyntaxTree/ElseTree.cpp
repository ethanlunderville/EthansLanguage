#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"


ElseTree::ElseTree() {}
void ElseTree::accept(ASTVisitor* v) { v->visitElseTree(this); }