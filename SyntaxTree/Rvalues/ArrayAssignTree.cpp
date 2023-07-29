#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include "SymbolTable/ContextManager.h"

ArrayAssignTree::ArrayAssignTree(std::string identifier, int line) {
    this->identifier = identifier;
    this->setLine(line);
}

std::string ArrayAssignTree::getIdentifier() {
    return this->identifier;
}

void ArrayAssignTree::checkType(Struct* structType){

}
//void ArrayAssignTree::checkFunction(Function* type){}
//void ArrayAssignTree::checkArray(Array* arrayType){}
void ArrayAssignTree::checkType(Number* numberType){
    std::vector<AST*> kids = this->getChildren();
    for (AST* potentialyValidChild : kids) {
        ExpressionTree* child = dynamic_cast<ExpressionTree*>(potentialyValidChild);
        if (child == nullptr) {
            std::cerr << "COMPILER ERROR::ALL ELEMENTS IN AN ARRAY ASSIGNMENT MUST BE EXPRESSION NODES" << std::endl;
        }
        this->getCheckerReference()->visitExpressionTree(child);
        std::any val = child->getVal();
        if (!numberType->checkType(child->getVal())) {
            std::cerr << "Invlaid array assignement on line " << this->getLine() 
            << " due to attempting to assign an array with at least one element of an incorrect type " << std::endl;
        }
    }
}
void ArrayAssignTree::checkType(String* stringType){

}
void ArrayAssignTree::assignType(Struct* type){}
//void ArrayAssignTree::assignFunction(Function* type){}
//void ArrayAssignTree::assignArray(Array* arrayType){}
void ArrayAssignTree::assignType(Number* arrayType){}
void ArrayAssignTree::assignType(String* arrayType){}

void ArrayAssignTree::accept(ASTVisitor* v) { v->visitArrayAssignTree(this); }