#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

StructExpressionTree::StructExpressionTree() : Evaluatable() {}

void StructExpressionTree::accept(ASTVisitor* v) { v->visitStructExpressionTree(this); }