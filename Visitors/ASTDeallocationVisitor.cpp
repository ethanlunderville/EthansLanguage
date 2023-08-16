#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

/*

    Deallocates Syntax Tree nodes recursively.

*/

ASTDeallocationVisitor::ASTDeallocationVisitor(){
    this->currentNodeNum = 1;
}

void ASTDeallocationVisitor::visitChildren(AST* astree){
    for (AST* child : astree->getChildren()) {
        (child)->accept(this);
    }
}

void ASTDeallocationVisitor::deallocate(AST* node) {
    this->visitChildren(node);
    #ifdef PRINTDEALLOCATIONADDRESSES
        std::cout << this->currentNodeNum << ". deleting node: " << node << std::endl;
    #endif
    delete node;
    node = nullptr;
    this->currentNodeNum++;
}

void ASTDeallocationVisitor::visitArrayAccessTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitFunctionAssignTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitStructAssignTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitArrayAssignTree (AST* astree) {deallocate(astree);}
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
void ASTDeallocationVisitor::visitFunctionDeclarationTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitStructDeclarationTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitArrayDeclarationTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitFunctionCallTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitWhileTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitElseTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitNumberTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitStringTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitIdentifierTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitGreaterTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitGreaterEqualTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitLessTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitLessEqualTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitEqualTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitNotEqualTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitAndTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitOrTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitAssignOpTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitArrowOpTree (AST* astree) {deallocate(astree);}
void ASTDeallocationVisitor::visitRegexSectionTree (AST* astree) {deallocate(astree);}