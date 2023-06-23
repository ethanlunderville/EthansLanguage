#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

/*
* OPERATOR CLASS DEFINED IN SyntaxTree/AST.h
*/
Operator::Operator() {
    precedence = 0;
}
Operator::~Operator() {}
short Operator::getPrecendence() {
    return this->precedence;
}