#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include "SymbolTable/ContextManager.h"

ASTInterpreter::ASTInterpreter(TypeManager* typeManager) {
    this->contextManager = new ContextManager(typeManager);
    this->typeManager = typeManager;
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

/* KIND OF HACKY (: */
void ASTInterpreter::visitFunctionChildren(AST* astree){
    for (AST* child : astree->getChildren()) {
        if (typeid(*child) == typeid(ReturnTree)) {
            this->visitChildren(child);
            if (child->getChildren().size() > 0 && typeid(*(child->getChildren()[0])) == typeid(ExpressionTree)) {
                this->currentFuncPtr->setVal(
                    dynamic_cast<ExpressionTree*>(child->getChildren()[0])->getVal()
                );
            } else {
                /* THERE SHOULD BE AT LEAST ONE RETURN EXPRESSION */
                exit(1);
            }
            this->returned = true;
            return;
        }    
        if (!this->returned) {
            (child)->accept(this);
        } else {
            return;
        }
    }
}

void ASTInterpreter::visitProgramTree (AST* astree) { this->visitChildren(astree); }

void ASTInterpreter::visitFunctionCallTree (AST* astree) {
    this->currentFuncPtr = dynamic_cast<FunctionCallTree*>(astree);
    this->returned = false; 
    this->visitFunctionChildren(astree); /* THE VAL OF ASTREE WILL BE SET IN THIS FUNCTION */
}

void ASTInterpreter::visitDeclarationTree (AST* astree) {
    DeclarationTree* t = ((DeclarationTree*)astree);
    Type* type = this->typeManager->getTypeHandler(t->getType());
    this->contextManager->declareSymbol(t->getLine(), t->getIdentifier(), type);
    this->visitChildren(astree);
    contextManager->printSymbolTable();
    return;
}
void ASTInterpreter::visitStructDeclarationTree (AST* astree) {
    this->typeManager->getTypeHandler(dynamic_cast<StructDeclarationTree*>(astree)->getType());
}
void ASTInterpreter::visitArrayDeclarationTree (AST* astree) {
    ArrayDeclarationTree* arrayTree = ((ArrayDeclarationTree*)astree);
    Type* primTypeReference = this->typeManager->getTypeHandler(arrayTree->getType());
    this->contextManager->declareSymbol(arrayTree->getLine(), arrayTree->getIdentifier(), new Array(primTypeReference));
    contextManager->printSymbolTable();
    this->visitChildren(astree);
    return;
}
void ASTInterpreter::visitFunctionDeclarationTree (AST* astree) {
    FunctionDeclarationTree* functionTree = ((FunctionDeclarationTree*)astree);
    Type* primTypeReference = this->typeManager->getTypeHandler(functionTree->getType());
    this->contextManager->declareSymbol(functionTree->getLine(), functionTree->getIdentifier(), new Function(primTypeReference));
    this->visitChildren(astree);
    if (functionTree->getChildren().size() != 0) {
        this->visitChildren(astree);
    }
    contextManager->printSymbolTable();
    return;
}




void ASTInterpreter::visitReturnTree (AST* astree) {

}











void ASTInterpreter::visitLValArrowOpTree(AST* astree){
    /*IF SOMETHING IS ON THE LEFT OF AN ARROW IT MUST BE IDENTIFIABLE*/
    Identifiable* eValType = dynamic_cast<Identifiable*>(astree->getChildren()[0]);
    if (eValType == nullptr) {
        std::cerr << "Non struct identifier with arrow following it" << std::endl;
        exit(1);
    } 
    Type* type;
    if (this->structScoper.size() > 0) {
        type = this->structScoper.top()->getTypeOfSymbol(eValType->getIdentifier());
    } else {
        type = this->contextManager->getTypeOfSymbol(eValType->getIdentifier());
    }
    if (typeid(*type) == typeid(Array) && typeid(*(eValType)) == typeid(ArrayAccessTree)) {
            type = dynamic_cast<Array*>(type)->getArrayType();
    } else if (typeid(*type) == typeid(Function)) {
        type = dynamic_cast<Function*>(type)->getFunctionType();
    }

    Struct* structType = dynamic_cast<Struct*>(type);
    if (structType == nullptr) { std::cerr << "COMPILER::ERROR -> STRUCT SHOULD HAVE BEEN HERE"; exit(1);}

    SymbolTable* structTable = structType->getBaseStructPointer();
    /*LEFT CHILD OF THIS TREE MUST ALWAYS BE AN IDENTIFIER WITH A STRUCT*/
    // IF IT IS ANOTHER ARROW RECURSIVELY CALL THIS METHOD ON IT
    if (typeid(*(astree->getChildren()[1])) == typeid(ArrowOpTree) ) {
        ArrowOpTree* arrowChild = dynamic_cast<ArrowOpTree*>(astree->getChildren()[1]);
        this->structScoper.push(structTable);
        this->visitLValArrowOpTree(arrowChild);
        dynamic_cast<Evaluatable*>(astree)->setVal(arrowChild->getVal());
        this->structScoper.pop();
    } else {
    // BASE CASE: IDENTIDIER HAS BEEN FOUND. PROCEED TO BUBBLE ITS TYPE TO THE TOP WITH RECURSION
        Identifiable* iDent = dynamic_cast<Identifiable*>(astree->getChildren()[1]);
        if (typeid(*iDent) == typeid(ArrayAccessTree)) {
            ExpressionTree* subTree = dynamic_cast<ExpressionTree*>(iDent->getChildren()[0]);
            subTree->accept(this);
            int subscript = std::any_cast<int>(subTree->getVal());
            dynamic_cast<ArrowOpTree*>(astree)->setVal(structTable->getReferenceOfValueStoredInSymbol(iDent->getIdentifier(), subscript));
            return;
        }
        dynamic_cast<ArrowOpTree*>(astree)->setVal(structTable->getReferenceOfValueStoredInSymbol(iDent->getIdentifier()));
    }
}
void ASTInterpreter::visitLValIdentifierTree(AST* astree){
    IdentifierTree* iTree = dynamic_cast<IdentifierTree*>(astree);
    iTree->setVal(this->contextManager->getReferenceOfValueStoredInSymbol(iTree->getIdentifier()));
}
void ASTInterpreter::visitLValArrayAccessTree(AST* astree){
    ArrayAccessTree* aAccessTree = dynamic_cast<ArrayAccessTree*>(astree);
    ExpressionTree* subTree = dynamic_cast<ExpressionTree*>(aAccessTree->getChildren()[0]);
    subTree->accept(this);
    int subscript = std::any_cast<int>(subTree->getVal());
    aAccessTree->setVal(this->contextManager->getReferenceOfValueStoredInSymbol(aAccessTree->getIdentifier(), subscript));
}
void ASTInterpreter::visitAssignOpTree (AST* astree) {
    AssignOpTree* aOpTree = dynamic_cast<AssignOpTree*>(astree);
    Evaluatable* lValTree = dynamic_cast<Evaluatable*>(astree->getChildren()[0]);
    lValTree->LVal = true;
    this->visitChildren(aOpTree);
    std::any* lVal = std::any_cast<std::any*>(lValTree->getVal());
    aOpTree->assign(
        lVal,
        dynamic_cast<Evaluatable*>(astree->getChildren()[1])->getVal()
    );
    this->contextManager->printSymbolTable();
}

void ASTInterpreter::visitIdentifierTree (AST* astree) {
    IdentifierTree* identer = dynamic_cast<IdentifierTree*>(astree);
    if (identer->LVal) {
        this->visitLValIdentifierTree(astree);
        return;
    }
    if (astree->getChildren().size() != 0) {
        ExpressionTree* subScript = dynamic_cast<ExpressionTree*>(astree->getChildren()[0]);
        this->visitExpressionTree(subScript);
        //identer->setVal(subScript->getVal());
        return;
    } 
    identer->setVal(this->contextManager->getValueStoredInSymbol(identer->getIdentifier()));
}

void ASTInterpreter::visitFunctionAssignTree(AST* astree) {
    dynamic_cast<Assignable*>(astree)->setVal(astree);
}
void ASTInterpreter::visitArrayAssignTree(AST* astree) {

}
void ASTInterpreter::visitStructAssignTree(AST* astree) {

}
void ASTInterpreter::visitExpressionTree (AST* astree) {
    ExpressionTree* t = ((ExpressionTree*)astree);
    this->visitChildren(t);
    t->setVal(dynamic_cast<Evaluatable*>(t->getChildren()[0])->getVal());
}

void ASTInterpreter::visitArrowOpTree (AST* astree) {

}
void ASTInterpreter::visitArrayAccessTree(AST* astree) {

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
void ASTInterpreter::visitElseTree (AST* astree) {
    this->visitChildren(astree);
}
void ASTInterpreter::visitWhileTree (AST* astree) {
    std::vector<AST*> children = astree->getChildren();
    ExpressionTree* conditionExpr = ((ExpressionTree*)(children[0]));
    this->visitExpressionTree(conditionExpr);
    while (std::any_cast<double>(conditionExpr->getVal())) {
        this->visitBlockTree((BlockTree*)(children[1]));
        this->visitExpressionTree(conditionExpr);
    }
}
void ASTInterpreter::visitBlockTree (AST* astree) {
    this->contextManager->pushScope();
    this->visitChildren(astree);
    this->contextManager->popScope(false);
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

void ASTInterpreter::visitPrintTree (AST* astree) { this->visitChildren(astree); }
void ASTInterpreter::visitNumberTree (AST* astree) {
    NumberTree* nTree = dynamic_cast<NumberTree*>(astree);
    nTree->setVal(nTree->getNumber());
}
void ASTInterpreter::visitStringTree (AST* astree) {}
