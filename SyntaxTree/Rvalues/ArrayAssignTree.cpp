#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ArrayAssignTree::ArrayAssignTree(std::string identifier, int line) {
    this->identifier = identifier;
    this->setLine(line);
}

std::string ArrayAssignTree::getIdentifier() {
    return this->identifier;
}

void ArrayAssignTree::checkType(Struct* structType){
    std::vector<AST*> kids = this->getChildren();
    for (AST* child : kids) {
        if (dynamic_cast<ExpressionTree*>(child) == nullptr) {
            std::cerr << "COMPILER ERROR::ALL ELEMENTS IN AN ARRAY ASSIGNMENT MUST BE EXPRESSION NODES" << std::endl;
        }
        if (!structType->checkExpression(child, this->getLine() , this->getCheckerReference())) {
            std::cerr << "Invlaid array assignement on line " << this->getLine() 
            << " due to attempting to assign an array with at least one element of an inccorect type " << std::endl;
        }
    }
}
//void ArrayAssignTree::checkFunction(Function* type){}
//void ArrayAssignTree::checkArray(Array* arrayType){}
void ArrayAssignTree::checkType(Number* numberType){
    std::vector<AST*> kids = this->getChildren();
    for (AST* child : kids) {
        if (dynamic_cast<ExpressionTree*>(child) == nullptr) {
            std::cerr << "COMPILER ERROR::ALL ELEMENTS IN AN ARRAY ASSIGNMENT MUST BE EXPRESSION NODES" << std::endl;
        }
        if (!numberType->checkExpression(child, this->getLine() , this->getCheckerReference())) {
            std::cerr << "Invlaid array assignement on line " << this->getLine() 
            << " due to attempting to assign an array with at least one element of an inccorect type " << std::endl;
        }
    }
}
void ArrayAssignTree::checkType(String* stringType){
    std::vector<AST*> kids = this->getChildren();
    for (AST* child : kids) {
        if (dynamic_cast<ExpressionTree*>(child) == nullptr) {
            std::cerr << "COMPILER ERROR::ALL ELEMENTS IN AN ARRAY ASSIGNMENT MUST BE EXPRESSION NODES" << std::endl;
        }
        if (!stringType->checkExpression(child, this->getLine() , this->getCheckerReference())) {
            std::cerr << "Invlaid array assignement on line " << this->getLine() 
            << " due to attempting to assign an array with at least one element of an inccorect type " << std::endl;
        }
    }
}
void ArrayAssignTree::assignType(Struct* type){}
//void ArrayAssignTree::assignFunction(Function* type){}
//void ArrayAssignTree::assignArray(Array* arrayType){}
void ArrayAssignTree::assignType(Number* arrayType){}
void ArrayAssignTree::assignType(String* arrayType){}

void ArrayAssignTree::accept(ASTVisitor* v) { v->visitArrayAssignTree(this); }