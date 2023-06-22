#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

LessTree::LessTree() {
    precedence = 0;
}
void LessTree::accept(ASTVisitor* v) { v->visitLessTree(this); }