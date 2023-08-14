#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ExpressionTree::ExpressionTree(int line) {
    this->setLine(line);
}

void ExpressionTree::accept(ASTVisitor* v) { v->visitExpressionTree(this); }