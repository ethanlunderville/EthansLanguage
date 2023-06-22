#include <cmath>
#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ASTDeallocationVisitor::ASTDeallocationVisitor(){
    this->nodenum = 1;
}

void ASTDeallocationVisitor::visitChildren(AST* astree){
    for (AST* child : astree->getChildren()) {
        (child)->accept(this);
    }
}

void ASTDeallocationVisitor::deallocate(AST* node) {
    this->visitChildren(node);
    std::cout << this->nodenum << ". deleting node: " << node << std::endl;
    delete node;
    node = nullptr;
    this->nodenum++;
}

void ASTDeallocationVisitor::visitAssignTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitIfTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitExpressionTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitDivideTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitMultiplyTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitAddTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitSubtractTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitExponentTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitDeclarationTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitBlockTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitReturnTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitProgramTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitFunctionTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitFunctionDeclarationTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitWhileTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitElseTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitNumberTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitStringTree (AST* astree) {deallocate(astree);}

void ASTDeallocationVisitor::visitGreaterTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitGreaterEqualTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitLessTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitLessEqualTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitEqualTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitNotEqualTree (AST* astree) {deallocate(astree);}
