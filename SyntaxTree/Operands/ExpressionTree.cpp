#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ExpressionTree::ExpressionTree() : Evaluatable() {}

void ExpressionTree::accept(ASTVisitor* v) { v->visitExpressionTree(this); }