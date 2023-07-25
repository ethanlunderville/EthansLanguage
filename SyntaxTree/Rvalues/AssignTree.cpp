#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AssignTree::AssignTree(std::string identifier, int line) {
    this->identifier = identifier;
    this->setLine(line);
}
std::string AssignTree::getIdentifier() {
    return this->identifier;
}

void AssignTree::checkType(Struct* type){}
//void AssignTree::checkFunction(Function* type){}
//void AssignTree::checkArray(Array* arrayType){}
void AssignTree::checkType(Number* arrayType){}
void AssignTree::checkType(String* arrayType){}
void AssignTree::assignType(Struct* type){}
//void AssignTree::assignFunction(Function* type){}
//void AssignTree::assignArray(Array* arrayType){}
void AssignTree::assignType(Number* arrayType){}
void AssignTree::assignType(String* arrayType){}

void AssignTree::accept(ASTVisitor* v) { v->visitAssignTree(this); }