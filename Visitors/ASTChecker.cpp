#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include "SymbolTable/ContextManager.h"
#include <cassert>

ASTChecker::ASTChecker() {
    this->contextManager = new ContextManager();
}
ASTChecker::~ASTChecker() {
    delete this->contextManager;
    this->contextManager = nullptr;
}
void ASTChecker::visitChildren(AST* astree){
    for (AST* child : astree->getChildren()) {
        (child)->accept(this);
    }
}
/*
void pushContext();
void popContext();
void pushScope();
void popScope();
void declareSymbol(int line, std::string identifier, std::string type);
void reassignSymbol(std::string identifier, std::any value, int line);
void printSymbolTable();
std::any getValueStoredInSymbol(std::string identifier);
std::string getTypeOfSymbol(std::string identifier);
*/
void ASTChecker::visitAssignTree(AST* astree) {
    AssignTree* t = ((AssignTree*)astree);
    assert(t->getChildren().size() > 0);
}
void ASTChecker::visitIfTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitExpressionTree (AST* astree) {
    
}
void ASTChecker::visitStringExpressionTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitDivideTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitMultiplyTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitAddTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitSubtractTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitExponentTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitDeclarationTree (AST* astree) {
    DeclarationTree* t = ((DeclarationTree*)astree);
    this->contextManager->declareSymbol(t->getLine(), t->getIdentifier(), t->getType());
    contextManager->printSymbolTable();
    if (t->getChildren().size() != 0) {
        this->visitChildren(astree);
    }
    contextManager->printSymbolTable();
    return;
}
void ASTChecker::visitBlockTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitReturnTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitProgramTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitFunctionTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitFunctionDeclarationTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitWhileTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitElseTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitNumberTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitStringTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitGreaterTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitGreaterEqualTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitLessTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitLessEqualTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitEqualTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitNotEqualTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitAndTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitOrTree (AST* astree) {this->visitChildren(astree);}