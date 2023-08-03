#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ArrayAccessTree::ArrayAccessTree(std::string identifier) {
    this->setIdentifier(identifier);
}

void ArrayAccessTree::accept(ASTVisitor* v) {
    v->visitArrayAccessTree(this);
}