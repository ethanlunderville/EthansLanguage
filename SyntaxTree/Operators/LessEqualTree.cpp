#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

LessEqualTree::LessEqualTree() {
    precedence = 1;
}
void LessEqualTree::accept(ASTVisitor* v) { v->visitLessEqualTree(this); }