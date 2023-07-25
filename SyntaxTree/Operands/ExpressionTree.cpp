#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ExpressionTree::ExpressionTree(int line) {
    this->setLine(line);
}

void ExpressionTree::checkType(Struct* structType){
    
}
//void ExpressionTree::checkFunction(Function* type){}
//void ExpressionTree::checkArray(Array* arrayType){}
void ExpressionTree::checkType(Number* numberType){
    
}
void ExpressionTree::checkType(String* stringType){
    if (!stringType->checkType(this)) {
        std::cerr << "Error, Invaldid string expression on line: " << this->getLine() << std::endl;
    }
}

void ExpressionTree::assignType(Struct* structType){}
//void ExpressionTree::assignFunction(Function* type){}
//void ExpressionTree::assignArray(Array* arrayType){}
void ExpressionTree::assignType(Number* numberType){}
void ExpressionTree::assignType(String* stringType){}

void ExpressionTree::accept(ASTVisitor* v) { v->visitExpressionTree(this); }