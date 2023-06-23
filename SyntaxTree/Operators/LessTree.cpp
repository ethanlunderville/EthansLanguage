#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

LessTree::LessTree() {
    precedence = 1;
}
void LessTree::accept(ASTVisitor* v) { v->visitLessTree(this); }