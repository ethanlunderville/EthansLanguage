#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

ASTPrintVisitor::ASTPrintVisitor(){
    this->indent = 1;
    this->lineNum = 1;
}

void ASTPrintVisitor::visitChildren(AST* astree){
    for (AST* child : astree->getChildren()) {
        (child)->accept(this);
        
    }
}

void ASTPrintVisitor::printIndent() {
    for (int i = 0 ; i < this->indent ; i++) {
        std::cout << " ";
    }
}

void ASTPrintVisitor::printer(std::string type, AST* node) {
    std::cout << this->lineNum << ".";
    printIndent();
    std::cout << type << ": " << node << std::endl;
    this->indent+=2;
    this->lineNum++;
    this->visitChildren(node);
    this->indent-=2;
}

void ASTPrintVisitor::visitAssignTree (AST* astree) { printer("AssignTree", astree);}
void ASTPrintVisitor::visitIfTree (AST* astree) {printer("IfTree", astree);}
void ASTPrintVisitor::visitExpressionTree (AST* astree) {printer("ExpressionTree", astree);}
void ASTPrintVisitor::visitDivideTree (AST* astree) {printer("DivideTree", astree);}
void ASTPrintVisitor::visitMultiplyTree (AST* astree) {printer("MultiplyTree", astree);}
void ASTPrintVisitor::visitAddTree (AST* astree) {printer("AddTree", astree);}
void ASTPrintVisitor::visitSubtractTree (AST* astree) {printer("SubtractTree", astree);}
//void ASTPrintVisitor::visitAST (AST* astree) {printer("AST", astree);}
void ASTPrintVisitor::visitDeclarationTree (AST* astree) {printer("DeclarationTree", astree);}
void ASTPrintVisitor::visitBlockTree (AST* astree) {printer("BlockTree", astree);}
void ASTPrintVisitor::visitReturnTree (AST* astree) {printer("ReturnTree", astree);}
void ASTPrintVisitor::visitProgramTree (AST* astree) {printer("ProgramTree", astree);}
void ASTPrintVisitor::visitFunctionTree (AST* astree) {printer("FunctionTree", astree);}
void ASTPrintVisitor::visitFunctionDeclarationTree (AST* astree) {printer("FunctionDeclTree", astree);}
void ASTPrintVisitor::visitWhileTree (AST* astree) {printer("WhileTree", astree);}
void ASTPrintVisitor::visitElseTree (AST* astree) {printer("ElseTree", astree);}
void ASTPrintVisitor::visitNumberTree (AST* astree) {printer("StringTree", astree);}
void ASTPrintVisitor::visitStringTree (AST* astree) {printer("NumberTree", astree);}
