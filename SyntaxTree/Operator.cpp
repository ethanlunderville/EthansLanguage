#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

Operator::Operator() {
    precedence = 0;
}

Operator::~Operator() {}

short Operator::getPrecendence() {
    return this->precedence;
}