#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ExpressionTree::ExpressionTree() : Assignable() {}

void ExpressionTree::checkStruct(Struct* type){}
void ExpressionTree::checkFunction(Function* type){}
void ExpressionTree::checkArray(Array* arrayType){}
void ExpressionTree::checkNumber(Number* arrayType){}
void ExpressionTree::checkString(String* arrayType){}
void ExpressionTree::assignStruct(Struct* type){}
void ExpressionTree::assignFunction(Function* type){}
void ExpressionTree::assignArray(Array* arrayType){}
void ExpressionTree::assignNumber(Number* arrayType){}
void ExpressionTree::assignString(String* arrayType){}

void ExpressionTree::accept(ASTVisitor* v) { v->visitExpressionTree(this); }