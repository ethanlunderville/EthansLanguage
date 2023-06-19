#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
AST::AST() {}
AST::~AST() {}

void AST::addChild(AST* tree) {
    children.push_back(tree);
}

std::vector<AST*> AST::getChildren() {
    return children;
}

//void AST::accept(ASTVisitor* v) { 
    //v->visitAST(this); 
//}