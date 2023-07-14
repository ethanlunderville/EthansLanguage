#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include "SymbolTable/ContextManager.h"

ASTInterpreter::ASTInterpreter(TypeManager* typeManager) {
    this->contextManager = new ContextManager(typeManager);
}
ASTInterpreter::~ASTInterpreter() {
    delete this->contextManager;
    this->contextManager = nullptr;
}

void ASTInterpreter::visitChildren(AST* astree){
    for (AST* child : astree->getChildren()) {
        (child)->accept(this);
    }
}
void ASTInterpreter::visitProgramTree (AST* astree) {
    this->visitChildren(astree);
    return;
}
void ASTInterpreter::visitAssignTree(AST* astree) {
    AssignTree* t = ((AssignTree*)astree);
    this->visitChildren(t);
    this->contextManager->reassignSymbol(t->getIdentifier(), ((ExpressionTree*)(t->getChildren()[0]))->getVal(), t->getLine());
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

void ASTInterpreter::visitDeclarationTree (AST* astree) {
    DeclarationTree* t = ((DeclarationTree*)astree);
    this->contextManager->declareSymbol(t->getLine(), t->getIdentifier(), t->getType());
    if (t->getChildren().size() != 0) {
        this->visitChildren(astree);
    }
    contextManager->printSymbolTable();
    return;
}
void ASTInterpreter::visitIfTree (AST* astree) {
    std::vector<AST*> children = astree->getChildren();
    ExpressionTree* conditionExpr = ((ExpressionTree*)(children[0]));
    this->visitExpressionTree(conditionExpr);
    if (std::any_cast<double>(conditionExpr->getVal())) {
        this->visitBlockTree((BlockTree*)(children[1]));
    } else if (children.size() == 3) {
        this->visitElseTree((ElseTree*)(children[2]));
    }
    contextManager->printSymbolTable();
}
void ASTInterpreter::visitBlockTree (AST* astree) {
    this->contextManager->pushScope();
    this->visitChildren(astree);
    this->contextManager->popScope();
}
void ASTInterpreter::visitReturnTree (AST* astree) {

}
void ASTInterpreter::visitFunctionTree (AST* astree) {

}
void ASTInterpreter::visitFunctionDeclarationTree (AST* astree) {

}
void ASTInterpreter::visitWhileTree (AST* astree) {

}
void ASTInterpreter::visitElseTree (AST* astree) {
    this->visitChildren(astree);
}
void ASTInterpreter::visitNumberTree (AST* astree) {}
void ASTInterpreter::visitStringTree (AST* astree) {}
void ASTInterpreter::visitIdentifierTree (AST* astree) {
    IdentifierTree* iTree = (IdentifierTree*)astree;
    iTree->setVal(this->contextManager->getValueStoredInSymbol((iTree)->getIdentifier()));
}
void ASTInterpreter::visitDivideTree (AST* astree) {
    DivideTree* t = ((DivideTree*)astree);
    this->visitChildren(t);
    std::any operand1 = ((Evaluatable*)(t->getChildren()[0]))->getVal();
    std::any operand2 = ((Evaluatable*)(t->getChildren()[1]))->getVal();
    t->divide(operand1, operand2);
}
void ASTInterpreter::visitMultiplyTree (AST* astree) {
    MultiplyTree* t = ((MultiplyTree*)astree);
    this->visitChildren(t);
    std::any operand1 = ((Evaluatable*)(t->getChildren()[0]))->getVal();
    std::any operand2 = ((Evaluatable*)(t->getChildren()[1]))->getVal();
    t->multiply(operand1, operand2);
}
void ASTInterpreter::visitAddTree (AST* astree) {
    AddTree* t = ((AddTree*)astree);
    this->visitChildren(t);
    std::any operand1 = ((Evaluatable*)(t->getChildren()[0]))->getVal();
    std::any operand2 = ((Evaluatable*)(t->getChildren()[1]))->getVal();
    t->add(operand1, operand2);
}
void ASTInterpreter::visitSubtractTree (AST* astree) {
    SubtractTree* t = ((SubtractTree*)astree);
    this->visitChildren(t);
    std::any operand1 = ((Evaluatable*)(t->getChildren()[0]))->getVal();
    std::any operand2 = ((Evaluatable*)(t->getChildren()[1]))->getVal();
    t->subtract(operand1,operand2);
}
void ASTInterpreter::visitExponentTree (AST* astree) {
    ExponentTree* t = ((ExponentTree*)astree);
    this->visitChildren(t);
    std::any operand1 = ((Evaluatable*)(t->getChildren()[0]))->getVal();
    std::any operand2 = ((Evaluatable*)(t->getChildren()[1]))->getVal();
    t->exponent(operand1,operand2);
}
void ASTInterpreter::visitGreaterTree (AST* astree) {
    GreaterTree* t = ((GreaterTree*)astree);
    this->visitChildren(t);
    std::any operand1 = ((Evaluatable*)(t->getChildren()[0]))->getVal();
    std::any operand2 = ((Evaluatable*)(t->getChildren()[1]))->getVal();
    t->greaterThan(operand1, operand2);
}
void ASTInterpreter::visitGreaterEqualTree (AST* astree) {
    GreaterEqualTree* t = ((GreaterEqualTree*)astree);
    this->visitChildren(t);
    std::any operand1 = ((Evaluatable*)(t->getChildren()[0]))->getVal();
    std::any operand2 = ((Evaluatable*)(t->getChildren()[1]))->getVal();
    t->greaterEqual(operand1, operand2);
}
void ASTInterpreter::visitLessTree (AST* astree) {
    LessTree* t = ((LessTree*)astree);
    this->visitChildren(t);
    std::any operand1 = ((Evaluatable*)(t->getChildren()[0]))->getVal();
    std::any operand2 = ((Evaluatable*)(t->getChildren()[1]))->getVal();
    t->lessThan(operand1, operand2);
}
void ASTInterpreter::visitLessEqualTree (AST* astree) {
    LessEqualTree* t = ((LessEqualTree*)astree);
    this->visitChildren(t);
    std::any operand1 = ((Evaluatable*)(t->getChildren()[0]))->getVal();
    std::any operand2 = ((Evaluatable*)(t->getChildren()[1]))->getVal();
    t->lessEqual(operand1, operand2);
}
void ASTInterpreter::visitEqualTree (AST* astree) {
    EqualTree* t = ((EqualTree*)astree);
    this->visitChildren(t);
    std::any operand1 = ((Evaluatable*)(t->getChildren()[0]))->getVal();
    std::any operand2 = ((Evaluatable*)(t->getChildren()[1]))->getVal();
    t->equal(operand1, operand2);
}
void ASTInterpreter::visitNotEqualTree (AST* astree) {
    NotEqualTree* t = ((NotEqualTree*)astree);
    this->visitChildren(t);
    std::any operand1 = ((Evaluatable*)(t->getChildren()[0]))->getVal();
    std::any operand2 = ((Evaluatable*)(t->getChildren()[1]))->getVal();
    t->notEqual(operand1, operand2);
}
void ASTInterpreter::visitAndTree (AST* astree) {
    AndTree* t = ((AndTree*)astree);
    this->visitChildren(t);
    std::any operand1 = ((Evaluatable*)(t->getChildren()[0]))->getVal();
    std::any operand2 = ((Evaluatable*)(t->getChildren()[1]))->getVal();
    t->opAnd(operand1, operand2);
}
void ASTInterpreter::visitOrTree (AST* astree) {
    OrTree* t = ((OrTree*)astree);
    this->visitChildren(t);
    std::any operand1 = ((Evaluatable*)(t->getChildren()[0]))->getVal();
    std::any operand2 = ((Evaluatable*)(t->getChildren()[1]))->getVal();
    t->opOr(operand1, operand2);
}
