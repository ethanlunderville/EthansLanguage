#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ArrowOpTree::ArrowOpTree() : Operator() {
    this->setPrecedence(5);
}

std::any ArrowOpTree::getOp(std::any x, std::any y) {
    return 1; 
}

void ArrowOpTree::accept(ASTVisitor* v) { v->visitArrowOpTree(this); }