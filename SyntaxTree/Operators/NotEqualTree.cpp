#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

NotEqualTree::NotEqualTree() {
    precedence = 1;
}
void NotEqualTree::accept(ASTVisitor* v) { v->visitNotEqualTree(this); }