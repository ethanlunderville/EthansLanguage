#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

LessEqualTree::LessEqualTree() {
    precedence = 0;
}
void LessEqualTree::accept(ASTVisitor* v) { v->visitLessEqualTree(this); }