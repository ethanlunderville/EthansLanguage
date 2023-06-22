#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

NotEqualTree::NotEqualTree() {
    precedence = 0;
}
void NotEqualTree::accept(ASTVisitor* v) { v->visitNotEqualTree(this); }