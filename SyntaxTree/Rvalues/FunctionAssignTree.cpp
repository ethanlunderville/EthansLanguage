#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

FunctionAssignTree::FunctionAssignTree(std::string identifier, int line) {
    this->identifier = identifier;
    this->line = line;
}

int FunctionAssignTree::getLine() {
    return this->line;
}

std::string FunctionAssignTree::getIdentifier() {
    return this->identifier;
}

void FunctionAssignTree::checkStruct(Struct* type){}
void FunctionAssignTree::checkFunction(Function* type){}
void FunctionAssignTree::checkArray(Array* arrayType){}
void FunctionAssignTree::checkNumber(Number* arrayType){}
void FunctionAssignTree::checkString(String* arrayType){}
void FunctionAssignTree::assignStruct(Struct* type){}
void FunctionAssignTree::assignFunction(Function* type){}
void FunctionAssignTree::assignArray(Array* arrayType){}
void FunctionAssignTree::assignNumber(Number* arrayType){}
void FunctionAssignTree::assignString(String* arrayType){}

void FunctionAssignTree::accept(ASTVisitor* v) { v->visitFunctionAssignTree(this); }