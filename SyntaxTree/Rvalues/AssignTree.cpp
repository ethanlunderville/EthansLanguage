#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AssignTree::AssignTree(std::string identifier, int line) {
    this->identifier = identifier;
    this->line = line;
}

int AssignTree::getLine() {
    return this->line;
}

std::string AssignTree::getIdentifier() {
    return this->identifier;
}

void AssignTree::checkStruct(Struct* type){}
void AssignTree::checkFunction(Function* type){}
void AssignTree::checkArray(Array* arrayType){}
void AssignTree::checkNumber(Number* arrayType){}
void AssignTree::checkString(String* arrayType){}
void AssignTree::assignStruct(Struct* type){}
void AssignTree::assignFunction(Function* type){}
void AssignTree::assignArray(Array* arrayType){}
void AssignTree::assignNumber(Number* arrayType){}
void AssignTree::assignString(String* arrayType){}

void AssignTree::accept(ASTVisitor* v) { v->visitAssignTree(this); }