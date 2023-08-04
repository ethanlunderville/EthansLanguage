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
        if (this->contextManager->isFunctionReturned()) {
            return;
        }
        (child)->accept(this);
    }
}

void ASTInterpreter::visitProgramTree (AST* astree) { this->visitChildren(astree); }

void ASTInterpreter::visitDeclarationTree (AST* astree) {
    DeclarationTree* t = ((DeclarationTree*)astree);
    Type* type = this->typeManager->getTypeHandler(t->getType());
    this->contextManager->declareSymbol(t->getLine(), t->getIdentifier(), type);
    this->visitChildren(astree);
    contextManager->printSymbolTable();
    return;
}
void ASTInterpreter::visitStructDeclarationTree (AST* astree) {
    this->typeManager->getTypeHandler(dynamic_cast<StructDeclarationTree*>(astree)->getIdentifier());
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
    contextManager->printSymbolTable();
    return;
}

void ASTInterpreter::visitReturnTree (AST* astree) {
    ExpressionTree* retExpr = dynamic_cast<ExpressionTree*>(astree->getChildren()[0]);
    retExpr->accept(this);
    this->currentFuncPtr->setVal(retExpr->getVal());
    this->contextManager->setFunctionIsReturned(true);
}

void ASTInterpreter::visitFunctionCallTree (AST* astree) {
    FunctionCallTree* fCallTree = dynamic_cast<FunctionCallTree*>(astree);
    this->currentFuncPtr = fCallTree;
    this->visitChildren(fCallTree); /*PREPARE THE FUNCTION ARGS BEFORE NEW CONTEXT GETS PUSHED*/
    std::any functionHold = this->contextManager->getValueStoredInSymbol(fCallTree->getIdentifier());
    FunctionAssignTree* actualFunction = std::any_cast<FunctionAssignTree*>(functionHold);
    std::vector<AST*>& parameterVector = actualFunction->getChildren();
    std::vector<AST*>& argumentVector = fCallTree->getChildren();
    this->contextManager->pushContext();
    AssignOpTree* aOp = new AssignOpTree();
    for (int i = 0 ; i < argumentVector.size() ; i++) {
        Declarable* declarer = dynamic_cast<Declarable*>(parameterVector[i]);
        this->contextManager->declareSymbol(
            declarer->getLine(),
            declarer->getIdentifier(),
            this->typeManager->getTypeHandler(declarer->getType())
        );
        aOp->assign( 
            this->contextManager->getReferenceOfValueStoredInSymbol(declarer->getIdentifier()),
            dynamic_cast<ExpressionTree*>(argumentVector[i])->getVal() 
        );
    }
    delete aOp;
    aOp = nullptr;
    BlockTree* bTree = dynamic_cast<BlockTree*>(parameterVector[parameterVector.size()-1]);
    this->contextManager->pushScope();
    this->visitChildren(bTree);
    this->contextManager->popScope(false);
    //this->visitBlockTree(parameterVector[parameterVector.size()-1]);
    this->contextManager->popContext();
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
    aOpTree->assign(
        std::any_cast<std::any*>(lValTree->getVal()),
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
    identer->setVal(this->contextManager->getValueStoredInSymbol(identer->getIdentifier()));
}

void ASTInterpreter::visitFunctionAssignTree(AST* astree) {
    dynamic_cast<FunctionAssignTree*>(astree)
    ->setVal(dynamic_cast<FunctionAssignTree*>(astree));
}

void ASTInterpreter::visitArrayAssignTree(AST* astree) {
    ArrayAssignTree* aTree = dynamic_cast<ArrayAssignTree*>(astree);
    std::vector<std::any> array;
    for (AST* eTree : aTree->getChildren()) {
        this->visitExpressionTree(eTree);
        array.push_back(dynamic_cast<ExpressionTree*>(eTree)->getVal());
    }
    aTree->setVal(array);
}
void ASTInterpreter::visitStructAssignTree(AST* astree) {
    StructAssignTree* sAssignTree = dynamic_cast<StructAssignTree*>(astree);
    sAssignTree->setVal(dynamic_cast<Struct*>(
        this->typeManager->getTypeHandler(sAssignTree->getTypeName())
    )->getDuplicateBase());
}
void ASTInterpreter::visitExpressionTree (AST* astree) {
    ExpressionTree* t = ((ExpressionTree*)astree);
    this->visitChildren(t);
    t->setVal(dynamic_cast<Evaluatable*>(t->getChildren()[0])->getVal());
}

void ASTInterpreter::visitArrowOpTree (AST* astree) {

    if (dynamic_cast<ArrowOpTree*>(astree)->LVal) { return this->visitLValArrowOpTree(astree); }
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
    if (typeid(*(astree->getChildren()[1])) == typeid(ArrowOpTree) ) { /*CONTINUE RECURSION IF RIGHT CHILD IS ARROW*/
        ArrowOpTree* arrowChild = dynamic_cast<ArrowOpTree*>(astree->getChildren()[1]);
        this->structScoper.push(structTable);
        this->visitLValArrowOpTree(arrowChild);
        dynamic_cast<Evaluatable*>(astree)->setVal(arrowChild->getVal());
        this->structScoper.pop();
    } else { /* BASE CASE: END OF ARROW EXPRESSION. RETURN THE VALUE OF THE RIGHTMOST VALUE */
        Type* rType;
        Identifiable* iDent = dynamic_cast<Identifiable*>(astree->getChildren()[1]);
        if (iDent != nullptr) { 
            dynamic_cast<ArrowOpTree*>(astree)->setVal(structTable->getValueStoredInSymbol(iDent->getIdentifier()));
        } else {
            std::cerr << "Incorrect arrow expression" << std::endl;
            exit(1);
        }
    }

}
void ASTInterpreter::visitArrayAccessTree(AST* astree) {
    ArrayAccessTree* aAccessTree = dynamic_cast<ArrayAccessTree*>(astree);
    this->visitChildren(aAccessTree);
    int subscript = std::any_cast<int>(dynamic_cast<ExpressionTree*>(aAccessTree->getChildren()[0])->getVal());
    aAccessTree->setVal(this->contextManager->getValueStoredInSymbol(aAccessTree->getIdentifier(),subscript));
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
void ASTInterpreter::visitStringTree (AST* astree) {
    StringTree* sTree = dynamic_cast<StringTree*>(astree);
    sTree->setVal(sTree->getString());
}
