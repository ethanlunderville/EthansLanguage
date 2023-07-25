#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

StructAssignTree::StructAssignTree(std::string typeName ,int line) {
    this->typeName = typeName;
    this->setLine(line);
}
std::string StructAssignTree::getTypeName() {
    return this->typeName;
}

void StructAssignTree::checkType(Struct* type){}
//void StructAssignTree::checkFunction(Function* type){}
//void StructAssignTree::checkArray(Array* arrayType){}
void StructAssignTree::checkType(Number* arrayType){}
void StructAssignTree::checkType(String* arrayType){}
void StructAssignTree::assignType(Struct* type){}
//void StructAssignTree::assignFunction(Function* type){}
//void StructAssignTree::assignArray(Array* arrayType){}
void StructAssignTree::assignType(Number* arrayType){}
void StructAssignTree::assignType(String* arrayType){}

void StructAssignTree::accept(ASTVisitor* v) { v->visitStructAssignTree(this); }