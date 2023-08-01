#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ArrowOpTree::ArrowOpTree() : Operator() : type(nullptr) {
    this->setPrecedence(5);
}

std::any ArrowOpTree::getOp(std::any x, std::any y) {
    return 1; 
}

void ArrowOpTree::setType(Type* type) {
    this->type = type;
}

Type* ArrowOpTree::getType() {
    return this->type;
}

void ArrowOpTree::accept(ASTVisitor* v) { v->visitArrowOpTree(this); }