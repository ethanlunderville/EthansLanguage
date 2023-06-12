#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"


void ASTPrintVisitor::visitChildren(AST* astree){
    for (AST* child : astree->getChildren()) {
        (child)->accept(this);
    }
}

void printer(std::string type, AST* node) {
    
}

void ASTPrintVisitor::visitAssignTree (AST* astree) {}
void ASTPrintVisitor::visitIfTree (AST* astree) {}
void ASTPrintVisitor::visitExpressionTree (AST* astree) {}
void ASTPrintVisitor::visitDivideTree (AST* astree) {}
void ASTPrintVisitor::visitMultiplyTree (AST* astree) {}
void ASTPrintVisitor::visitAddTree (AST* astree) {}
void ASTPrintVisitor::visitSubtractTree (AST* astree) {}
void ASTPrintVisitor::visitAST (AST* astree) {}
void ASTPrintVisitor::visitDeclarationTree (AST* astree) {}
void ASTPrintVisitor::visitBlockTree (AST* astree) {}
void ASTPrintVisitor::visitReturnTree (AST* astree) {}
void ASTPrintVisitor::visitProgramTree (AST* astree) {}
void ASTPrintVisitor::visitFunctionTree (AST* astree) {}
void ASTPrintVisitor::visitFunctionDeclarationTree (AST* astree) {}
void ASTPrintVisitor::visitWhileTree (AST* astree) {}
void ASTPrintVisitor::visitElseTree (AST* astree) {}