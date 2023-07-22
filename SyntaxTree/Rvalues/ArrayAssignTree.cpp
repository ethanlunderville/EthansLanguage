#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ArrayAssignTree::ArrayAssignTree(std::string identifier, int line) {
    this->identifier = identifier;
    this->line = line;
}

int ArrayAssignTree::getLine() {
    return this->line;
}

std::string ArrayAssignTree::getIdentifier() {
    return this->identifier;
}

void ArrayAssignTree::checkStruct(Struct* type){}
void ArrayAssignTree::checkFunction(Function* type){}
void ArrayAssignTree::checkArray(Array* arrayType){}
void ArrayAssignTree::checkNumber(Number* arrayType){}
void ArrayAssignTree::checkString(String* arrayType){}
void ArrayAssignTree::assignStruct(Struct* type){}
void ArrayAssignTree::assignFunction(Function* type){}
void ArrayAssignTree::assignArray(Array* arrayType){}
void ArrayAssignTree::assignNumber(Number* arrayType){}
void ArrayAssignTree::assignString(String* arrayType){}

void ArrayAssignTree::accept(ASTVisitor* v) { v->visitArrayAssignTree(this); }