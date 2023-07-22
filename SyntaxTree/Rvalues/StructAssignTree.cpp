#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

StructAssignTree::StructAssignTree(std::string typeName ,int line) {
    this->typeName = typeName;
    this->line = line;
}

int StructAssignTree::getLine() {
    return this->line;
}

std::string StructAssignTree::getTypeName() {
    return this->typeName;
}

void StructAssignTree::checkStruct(Struct* type){}
void StructAssignTree::checkFunction(Function* type){}
void StructAssignTree::checkArray(Array* arrayType){}
void StructAssignTree::checkNumber(Number* arrayType){}
void StructAssignTree::checkString(String* arrayType){}
void StructAssignTree::assignStruct(Struct* type){}
void StructAssignTree::assignFunction(Function* type){}
void StructAssignTree::assignArray(Array* arrayType){}
void StructAssignTree::assignNumber(Number* arrayType){}
void StructAssignTree::assignString(String* arrayType){}

void StructAssignTree::accept(ASTVisitor* v) { v->visitStructAssignTree(this); }