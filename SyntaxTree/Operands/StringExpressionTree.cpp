#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
StringExpressionTree::StringExpressionTree() : ExpressionTree() {}
void StringExpressionTree::accept(ASTVisitor* v) { v->visitStringExpressionTree(this); }