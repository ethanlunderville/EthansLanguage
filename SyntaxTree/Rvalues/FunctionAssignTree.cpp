#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

FunctionAssignTree::FunctionAssignTree(std::string identifier, int line) {
    this->identifier = identifier;
    this->setLine(line);
}
std::string FunctionAssignTree::getIdentifier() {
    return this->identifier;
}

void FunctionAssignTree::checkType(Struct* type){}
//void FunctionAssignTree::checkFunction(Function* type){}
//void FunctionAssignTree::checkArray(Array* arrayType){}
void FunctionAssignTree::checkType(Number* arrayType){}
void FunctionAssignTree::checkType(String* arrayType){}

void FunctionAssignTree::assignType(Struct* type){}
//void FunctionAssignTree::assignFunction(Function* type){}
//void FunctionAssignTree::assignArray(Array* arrayType){}
void FunctionAssignTree::assignType(Number* arrayType){}
void FunctionAssignTree::assignType(String* arrayType){}

void FunctionAssignTree::accept(ASTVisitor* v) { v->visitFunctionAssignTree(this); }