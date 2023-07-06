#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include "SymbolTable/ContextManager.h"
#include <cassert>

ASTInterpreter::ASTInterpreter() {
    this->contextManager = new ContextManager();
}

void ASTInterpreter::visitChildren(AST* astree){
    for (AST* child : astree->getChildren()) {
        (child)->accept(this);
    }
}

void ASTInterpreter::visitAssignTree(AST* astree) {
    AssignTree* t = ((AssignTree*)astree);
    assert(t->getChildren().size() > 0);
    this->visitChildren(t);
    this->contextManager->reassignSymbol(t->getIdentifier(), ((ExpressionTree*)(t->getChildren()[0]))->getVal(), t->getLine());
}
void ASTInterpreter::visitIfTree (AST* astree) {
    

}
void ASTInterpreter::visitExpressionTree (AST* astree) {
    ExpressionTree* t = ((ExpressionTree*)astree);
    this->visitChildren(t);
    t->setVal( std::any_cast<double>((dynamic_cast<Evaluatable*>(t->getChildren()[0])->getVal())) );
}
void ASTInterpreter::visitStringExpressionTree (AST* astree) {
    ExpressionTree* t = ((ExpressionTree*)astree);
    this->visitChildren(t);
    t->setVal( std::any_cast<std::string>((dynamic_cast<Evaluatable*>(t->getChildren()[0])->getVal())) );
}
void ASTInterpreter::visitDivideTree (AST* astree) {

}
void ASTInterpreter::visitMultiplyTree (AST* astree) {

}
void ASTInterpreter::visitAddTree (AST* astree) {
    AddTree* t = ((AddTree*)astree);
    //t->visitChildren(this);
    //t->add(t->getChildren()[0], t->getChildren()[1]);
}
void ASTInterpreter::visitSubtractTree (AST* astree) {

}
void ASTInterpreter::visitExponentTree (AST* astree) {

}
void ASTInterpreter::visitDeclarationTree (AST* astree) {
    DeclarationTree* t = ((DeclarationTree*)astree);
    this->contextManager->declareSymbol(t->getLine(), t->getIdentifier(), t->getType());
    contextManager->printSymbolTable();
    if (t->getChildren().size() != 0) {
        this->visitChildren(astree);
    }
    contextManager->printSymbolTable();
    return;
}
void ASTInterpreter::visitBlockTree (AST* astree) {

}
void ASTInterpreter::visitReturnTree (AST* astree) {

}
void ASTInterpreter::visitProgramTree (AST* astree) {
    this->visitChildren(astree);
    return;
}
void ASTInterpreter::visitFunctionTree (AST* astree) {

}
void ASTInterpreter::visitFunctionDeclarationTree (AST* astree) {

}
void ASTInterpreter::visitWhileTree (AST* astree) {

}
void ASTInterpreter::visitElseTree (AST* astree) {

}
void ASTInterpreter::visitNumberTree (AST* astree) {

}
void ASTInterpreter::visitStringTree (AST* astree) {}
void ASTInterpreter::visitGreaterTree (AST* astree) {}
void ASTInterpreter::visitGreaterEqualTree (AST* astree) {}
void ASTInterpreter::visitLessTree (AST* astree) {}
void ASTInterpreter::visitLessEqualTree (AST* astree) {}
void ASTInterpreter::visitEqualTree (AST* astree) {}
void ASTInterpreter::visitNotEqualTree (AST* astree) {}
void ASTInterpreter::visitAndTree (AST* astree) {}
void ASTInterpreter::visitOrTree (AST* astree) {}