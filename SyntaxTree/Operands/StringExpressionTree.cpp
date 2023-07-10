#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

StringExpressionTree::StringExpressionTree() : Evaluatable() {}

void StringExpressionTree::accept(ASTVisitor* v) { v->visitStringExpressionTree(this); }