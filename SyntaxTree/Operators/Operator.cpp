#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

/*
* OPERATOR CLASS DEFINED IN SyntaxTree/AST.h
*/
Operator::Operator() : Evaluatable() {
    precedence = -1;
}
Operator::~Operator() {}

void Operator::setPrecendence(short precedence) {
    this->precedence = precedence;
}

short Operator::getPrecendence() {
    return this->precedence;
}

void Operator::accept(ASTVisitor* v) {return;}
