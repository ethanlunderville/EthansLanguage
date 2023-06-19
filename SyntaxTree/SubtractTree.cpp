#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

SubtractTree::SubtractTree() {
    precedence = 1;
}
void SubtractTree::accept(ASTVisitor* v) { v->visitSubtractTree(this); }