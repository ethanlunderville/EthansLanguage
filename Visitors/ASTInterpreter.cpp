#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include "SymbolTable/ContextManager.h"
#include "Library/Regex.cpp"

/*
    IMPORTANT: READ THE ASTChecker 

    Code for interpreting the AST

*/

ASTInterpreter::ASTInterpreter(TypeManager* typeManager) : 
    typeManager(typeManager),
    contextManager(new ContextManager(typeManager)),
    lValBubbler(new ASTChecker(typeManager)),
    temporary(false) {}

ASTInterpreter::ASTInterpreter(TypeManager* typeManager, ContextManager* contextManager) :
    typeManager(typeManager),
    contextManager(contextManager),
    lValBubbler(new ASTChecker(typeManager)),
    temporary(false) {}

ASTInterpreter::~ASTInterpreter() {
    delete this->lValBubbler;
    this->lValBubbler = nullptr;
    /* ASTCHECKER WILL INSTANTIATE AN INTERPRETER 
       TO ASSIGN FUNCTIONS USING ITS CONTEXT MANAGER.
       THIS TEMPORARY INTERPRETER SHOULD NOT DELETE
       THE CHECKERS CONTEXT MANAGER
    */
    if (!this->temporary) { 
        delete this->contextManager;
        this->contextManager = nullptr;
    }
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
    #ifdef PRINTDECSANDASSIGNMENTS
        this->printCallIndent();
        std::cout << "Declaring: " << t->getIdentifier() 
        << " at "
        << this->contextManager->getReferenceOfValueStoredInSymbol(t->getIdentifier()) 
        << std::endl;
    #endif
    #ifdef PRINTSYMBOLS
        contextManager->printSymbolTable();
    #endif
    return;
}

void ASTInterpreter::visitStructDeclarationTree (AST* astree) {
    this->typeManager->getTypeHandler(
        dynamic_cast<StructDeclarationTree*>(astree)->getIdentifier()
    );
}

void ASTInterpreter::visitArrayDeclarationTree (AST* astree) {
    ArrayDeclarationTree* arrayTree = ((ArrayDeclarationTree*)astree);
    Type* primTypeReference = this->typeManager->getTypeHandler(arrayTree->getType());
    this->contextManager->declareSymbol(
        arrayTree->getLine(), 
        arrayTree->getIdentifier(), 
        new Array(primTypeReference)
    );
    #ifdef PRINTDECSANDASSIGNMENTS
        this->printCallIndent();        
        std::cout << "Declaring: " << arrayTree->getIdentifier() 
        << " at "
        << this->contextManager->getReferenceOfValueStoredInSymbol(arrayTree->getIdentifier()) 
        << std::endl;
    #endif
    #ifdef PRINTSYMBOLS
        contextManager->printSymbolTable();
    #endif
    this->visitChildren(astree);
    return;
}

void ASTInterpreter::visitFunctionDeclarationTree (AST* astree) {
    FunctionDeclarationTree* functionTree = dynamic_cast<FunctionDeclarationTree*>(astree) ;
    Type* primTypeReference = this->typeManager->getTypeHandler(functionTree->getType());
    this->contextManager->declareSymbol(
        functionTree->getLine(),
        functionTree->getIdentifier(), 
        new Function(primTypeReference)
    );
    #ifdef PRINTDECSANDASSIGNMENTS
        this->printCallIndent();
        std::cout << "Declaring: " << functionTree->getIdentifier() 
        << " at "
        << this->contextManager->getReferenceOfValueStoredInSymbol(functionTree->getIdentifier()) 
        << std::endl;
    #endif
    #ifdef PRINTSYMBOLS
        contextManager->printSymbolTable();
    #endif
    return;
}

void ASTInterpreter::visitReturnTree (AST* astree) {
    ExpressionTree* retExpr = dynamic_cast<ExpressionTree*>(astree->getChildren()[0]);
    retExpr->accept(this);
    /*SETS THE VALUE OF THE FUNCTION PUSHED IN THE CORRESPONDING FUNCTION CALL*/
    this->callStack.top()->setVal(retExpr->getVal()); 
    #ifdef PRINTCALLSTACK
        this->printCallIndent();
        std::cout << "RETURNING ::: => ";
        if (retExpr->getVal().type() == typeid(double)) {
            std::cout << (std::any_cast<double>(retExpr->getVal()));
        } else if (retExpr->getVal().type() == typeid(std::string)) {
            std::cout << (std::any_cast<std::string>(retExpr->getVal()));
        } else {
            std::cout << retExpr->getVal().type().name();
        } 
        std::cout << " ::: " << callStack.top() << std::endl;
    #endif
    /*NO OTHER NODES IN FUNCTION WILL BE VISITED PLEASE SEE VISIT CHILDREN FUNCTION*/
    this->contextManager->setFunctionIsReturned(true); 
}

void ASTInterpreter::visitFunctionCallTree (AST* astree) {
    FunctionCallTree* fCallTree = dynamic_cast<FunctionCallTree*>(astree);
    this->visitChildren(fCallTree); /*ARGUEMENTS ARE EVALUATED*/

    #ifdef PRINTCALLSTACK
        this->printCallIndent();
        std::cout << "CALL ::: => " 
        <<fCallTree->getIdentifier()
        << " ::: " 
        << fCallTree << std::endl;
    #endif

    this->callStack.push(fCallTree);
    if (Builtins::builtInFunctions[fCallTree->getIdentifier()] != nullptr) { /*CHECK BUILTINS*/
        fCallTree->setVal( /* Interface to another realm */
            Builtins::builtInFunctions[fCallTree->getIdentifier()](fCallTree->getChildren())
        );
        this->callStack.pop();
        return;
    }
    FunctionAssignTree* actualFunction =  std::any_cast<FunctionAssignTree*>(
        this->contextManager->getValueStoredInSymbol(fCallTree->getIdentifier())
    );
    // ALL PARAMETERS SHOULD INHERIT DECLARABLE
    std::vector<AST*>& parameterVector = actualFunction->getChildren();
    // ALL ARGUMENTS SHOULD BE AN EXPRESSIONTREE EXCEPT FOR THE LAST ONE WHICH SHOULD BE A BLOCK
    std::vector<AST*>& argumentVector = fCallTree->getChildren();
    AssignOpTree* aOp = new AssignOpTree(); // TEMPORARY TREE TO ASSIGN ARGUMENTS TO PARAMTERS
    this->contextManager->pushContext(); 
    for (int i = 0 ; i < argumentVector.size() ; i++) { /* Assign parameters */
        Declarable* declarer = dynamic_cast<Declarable*>(parameterVector[i]);
        declarer->accept(this);
        aOp->assign( 
            this->contextManager->getReferenceOfValueStoredInSymbol(declarer->getIdentifier()),
            dynamic_cast<ExpressionTree*>(argumentVector[i])->getVal() 
        );
    }
    BlockTree* bTree = dynamic_cast<BlockTree*>(parameterVector[parameterVector.size()-1]); 
    this->visitChildren(bTree); /* EXECUTES THE FUNCTION */
    this->contextManager->popContext();
    delete aOp;
    this->callStack.pop();
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
            dynamic_cast<ArrowOpTree*>(astree)->setVal(
                structTable->getReferenceOfValueStoredInSymbol(
                    iDent->getIdentifier(), 
                    subscript
                )
            );
            return;
        }
        dynamic_cast<ArrowOpTree*>(astree)->setVal(structTable->getReferenceOfValueStoredInSymbol(iDent->getIdentifier()));
    }
}

void ASTInterpreter::visitLValIdentifierTree(AST* astree){
    IdentifierTree* iTree = dynamic_cast<IdentifierTree*>(astree);
    iTree->setVal(
        this->contextManager->getReferenceOfValueStoredInSymbol(iTree->getIdentifier())
    );
}

void ASTInterpreter::visitLValArrayAccessTree(AST* astree){
    ArrayAccessTree* aAccessTree = dynamic_cast<ArrayAccessTree*>(astree); /* ARRAY IDENTIFIER TREE */
    ExpressionTree* subTree = dynamic_cast<ExpressionTree*>(aAccessTree->getChildren()[0]); /* SUBSCRIPT TREE */
    this->visitChildren(aAccessTree);
    std::any address = this->contextManager->getReferenceOfValueStoredInSymbol(
        aAccessTree->getIdentifier(), /* ARRAY IDENTIFIER */
        static_cast<int>(std::any_cast<double>(subTree->getVal())) /* SUBSCRIPT*/
    );
    aAccessTree->setVal(address);
}

void ASTInterpreter::visitArrayAccessTree(AST* astree) {
    ArrayAccessTree* aAccessTree = dynamic_cast<ArrayAccessTree*>(astree);
    if (aAccessTree->getIsLVal()) {
        return this->visitLValArrayAccessTree(aAccessTree);
    }
    this->visitChildren(aAccessTree);
    std::any subAny = dynamic_cast<ExpressionTree*>(aAccessTree->getChildren()[0])->getVal();
    int subscript = std::any_cast<double>(subAny);
    aAccessTree->setVal(
        this->contextManager->getValueStoredInSymbol(
            aAccessTree->getIdentifier(),
            subscript
        )
    );
}

void ASTInterpreter::visitAssignOpTree (AST* astree) {
    AssignOpTree* aOpTree = dynamic_cast<AssignOpTree*>(astree);
    Evaluatable* lValTree = dynamic_cast<Evaluatable*>(astree->getChildren()[0]);
    lValTree->setIsLVal(true);
    (astree->getChildren()[1])->accept(this); //SECOND CHILD MUST BE VISITED FIRST
    (astree->getChildren()[0])->accept(this);
    aOpTree->assign(
        std::any_cast<std::any*>(lValTree->getVal()), 
        dynamic_cast<Evaluatable*>(astree->getChildren()[1])->getVal()
    );
    #ifdef PRINTDECSANDASSIGNMENTS
        this->printCallIndent();
        std::cout << "Assigning: ";
        if (dynamic_cast<Evaluatable*>(astree->getChildren()[1])->getVal().type() == typeid(double)) {
            std::cout << (std::any_cast<double>(dynamic_cast<Evaluatable*>(astree->getChildren()[1])->getVal()));
        } else if (dynamic_cast<Evaluatable*>(astree->getChildren()[1])->getVal().type() == typeid(std::string)) {
            std::cout << (std::any_cast<std::string>(dynamic_cast<Evaluatable*>(astree->getChildren()[1])->getVal()));
        } else {
            std::cout << dynamic_cast<Evaluatable*>(astree->getChildren()[1])->getVal().type().name();
        } 
        std:: cout << " to address "<< std::any_cast<std::any*>(lValTree->getVal()) << std::endl;
    #endif
    #ifdef PRINTSYMBOLS
        contextManager->printSymbolTable();
    #endif
}

void ASTInterpreter::visitIdentifierTree (AST* astree) {
    IdentifierTree* identer = dynamic_cast<IdentifierTree*>(astree);
    if (identer->getIsLVal()) {
        this->visitLValIdentifierTree(astree);
        return;
    }
    identer->setVal(
        this->contextManager->getValueStoredInSymbol(
            identer->getIdentifier()
        )
    );
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

    if (dynamic_cast<ArrowOpTree*>(astree)->getIsLVal()) { return this->visitLValArrowOpTree(astree); }
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
        Identifiable* iDent = dynamic_cast<Identifiable*>(astree->getChildren()[1]);
        if (iDent != nullptr) { 
            if (typeid(*iDent) == typeid(FunctionCallTree)) {
                this->contextManager->pushScope();
                this->contextManager->declareSymbol(0, std::string("this"), this->typeManager->getTypeHandler(structTable->getStructName()));
                this->contextManager->reassignSymbol(std::string("this"), structTable, 0);
                iDent->accept(this);
                this->contextManager->popScope(false);
            }
            dynamic_cast<ArrowOpTree*>(astree)->setVal(structTable->getValueStoredInSymbol(iDent->getIdentifier()));
        } else {
            std::cerr << "Incorrect arrow expression, expectes the left most element to be identifiable" << std::endl;
            exit(1);
        }
    }

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
}
/*IF TREE DETERMINES WHETHER THIS TREE IS VISITED OR NOT*/
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

/* 
    IMPORTANT :: DO NOT USE THE VISITCHILDREN FUNCTION ON 
    OPERATORS IT CREATES ISSUES WHEN THE OPERANDS ARE THE
    SAME FUNCTION 
*/

void ASTInterpreter::visitDivideTree (AST* astree) {
    DivideTree* t = ((DivideTree*)astree);
    Evaluatable* op1 = dynamic_cast<Evaluatable*>(t->getChildren()[0]);
    op1->accept(this);
    std::any operand1 = op1->getVal();
    Evaluatable* op2 = dynamic_cast<Evaluatable*>(t->getChildren()[1]);
    op2->accept(this);
    std::any operand2 = op2->getVal();
    t->divide(operand1, operand2);
}

void ASTInterpreter::visitMultiplyTree (AST* astree) {
    MultiplyTree* t = ((MultiplyTree*)astree);
    Evaluatable* op1 = dynamic_cast<Evaluatable*>(t->getChildren()[0]);
    op1->accept(this);
    std::any operand1 = op1->getVal();
    Evaluatable* op2 = dynamic_cast<Evaluatable*>(t->getChildren()[1]);
    op2->accept(this);
    std::any operand2 = op2->getVal();
    t->multiply(operand1, operand2);
}

void ASTInterpreter::visitAddTree (AST* astree) {
    AddTree* t = ((AddTree*)astree);
    Evaluatable* op1 = dynamic_cast<Evaluatable*>(t->getChildren()[0]);
    op1->accept(this);
    std::any operand1 = op1->getVal();
    Evaluatable* op2 = dynamic_cast<Evaluatable*>(t->getChildren()[1]);
    op2->accept(this);
    std::any operand2 = op2->getVal();
    t->add(operand1, operand2);
}

void ASTInterpreter::visitSubtractTree (AST* astree) {
    SubtractTree* t = ((SubtractTree*)astree);
    this->visitChildren(t);
    Evaluatable* op1 = dynamic_cast<Evaluatable*>(t->getChildren()[0]);
    op1->accept(this);
    std::any operand1 = op1->getVal();
    Evaluatable* op2 = dynamic_cast<Evaluatable*>(t->getChildren()[1]);
    op2->accept(this);
    std::any operand2 = op2->getVal();
    t->subtract(operand1,operand2);
}

void ASTInterpreter::visitExponentTree (AST* astree) {
    ExponentTree* t = ((ExponentTree*)astree);
    Evaluatable* op1 = dynamic_cast<Evaluatable*>(t->getChildren()[0]);
    op1->accept(this);
    std::any operand1 = op1->getVal();
    Evaluatable* op2 = dynamic_cast<Evaluatable*>(t->getChildren()[1]);
    op2->accept(this);
    std::any operand2 = op2->getVal();
    t->exponent(operand1,operand2);
}

void ASTInterpreter::visitGreaterTree (AST* astree) {
    GreaterTree* t = ((GreaterTree*)astree);
    this->visitChildren(t);
    Evaluatable* op1 = dynamic_cast<Evaluatable*>(t->getChildren()[0]);
    op1->accept(this);
    std::any operand1 = op1->getVal();
    Evaluatable* op2 = dynamic_cast<Evaluatable*>(t->getChildren()[1]);
    op2->accept(this);
    std::any operand2 = op2->getVal();
    t->greaterThan(operand1, operand2);
}

void ASTInterpreter::visitGreaterEqualTree (AST* astree) {
    GreaterEqualTree* t = ((GreaterEqualTree*)astree);
    Evaluatable* op1 = dynamic_cast<Evaluatable*>(t->getChildren()[0]);
    op1->accept(this);
    std::any operand1 = op1->getVal();
    Evaluatable* op2 = dynamic_cast<Evaluatable*>(t->getChildren()[1]);
    op2->accept(this);
    std::any operand2 = op2->getVal();
    t->greaterEqual(operand1, operand2);
}

void ASTInterpreter::visitLessTree (AST* astree) {
    LessTree* t = ((LessTree*)astree);
    Evaluatable* op1 = dynamic_cast<Evaluatable*>(t->getChildren()[0]);
    op1->accept(this);
    std::any operand1 = op1->getVal();
    Evaluatable* op2 = dynamic_cast<Evaluatable*>(t->getChildren()[1]);
    op2->accept(this);
    std::any operand2 = op2->getVal();
    t->lessThan(operand1, operand2);
}

void ASTInterpreter::visitLessEqualTree (AST* astree) {
    LessEqualTree* t = ((LessEqualTree*)astree);
    Evaluatable* op1 = dynamic_cast<Evaluatable*>(t->getChildren()[0]);
    op1->accept(this);
    std::any operand1 = op1->getVal();
    Evaluatable* op2 = dynamic_cast<Evaluatable*>(t->getChildren()[1]);
    op2->accept(this);
    std::any operand2 = op2->getVal();
    t->lessEqual(operand1, operand2);
}

void ASTInterpreter::visitEqualTree (AST* astree) {
    EqualTree* t = ((EqualTree*)astree);
    Evaluatable* op1 = dynamic_cast<Evaluatable*>(t->getChildren()[0]);
    op1->accept(this);
    std::any operand1 = op1->getVal();
    Evaluatable* op2 = dynamic_cast<Evaluatable*>(t->getChildren()[1]);
    op2->accept(this);
    std::any operand2 = op2->getVal();
    t->equal(operand1, operand2);
}

void ASTInterpreter::visitNotEqualTree (AST* astree) {
    NotEqualTree* t = ((NotEqualTree*)astree);
    Evaluatable* op1 = dynamic_cast<Evaluatable*>(t->getChildren()[0]);
    op1->accept(this);
    std::any operand1 = op1->getVal();
    Evaluatable* op2 = dynamic_cast<Evaluatable*>(t->getChildren()[1]);
    op2->accept(this);
    std::any operand2 = op2->getVal();
    t->notEqual(operand1, operand2);
}

void ASTInterpreter::visitAndTree (AST* astree) {
    AndTree* t = ((AndTree*)astree);
    Evaluatable* op1 = dynamic_cast<Evaluatable*>(t->getChildren()[0]);
    op1->accept(this);
    std::any operand1 = op1->getVal();
    Evaluatable* op2 = dynamic_cast<Evaluatable*>(t->getChildren()[1]);
    op2->accept(this);
    std::any operand2 = op2->getVal();
    t->opAnd(operand1, operand2);
}

void ASTInterpreter::visitOrTree (AST* astree) {
    OrTree* t = ((OrTree*)astree);
    Evaluatable* op1 = dynamic_cast<Evaluatable*>(t->getChildren()[0]);
    op1->accept(this);
    std::any operand1 = op1->getVal();
    Evaluatable* op2 = dynamic_cast<Evaluatable*>(t->getChildren()[1]);
    op2->accept(this);
    std::any operand2 = op2->getVal();
    t->opOr(operand1, operand2);
}

void ASTInterpreter::visitNumberTree (AST* astree) {
    NumberTree* nTree = dynamic_cast<NumberTree*>(astree);
    nTree->setVal(nTree->getNumber());
}

void ASTInterpreter::visitStringTree (AST* astree) {
    StringTree* sTree = dynamic_cast<StringTree*>(astree);
    sTree->setVal(sTree->getString());
}

void ASTInterpreter::visitRegexSectionTree(AST* astree) {
    std::any inputAny = this->contextManager->getValueStoredInSymbol(
        dynamic_cast<RegexSectionTree*>(astree)->getRegex()
    );
    std::string input = std::any_cast<std::string>(inputAny);
    std::vector<AST*> regexSections = astree->getChildren();
    RegexInterpreterManager* rManage = new RegexInterpreterManager();
    for (int i = 0 ; i < regexSections.size() ; i++) {
        std::string inputRegex = dynamic_cast<RegexSectionTree*>(regexSections[i])->getRegex();
        inputRegex = inputRegex.substr(1, inputRegex.size() - 2);
        rManage->runRegexSection(
            inputRegex, 
            input, 
            this, 
            regexSections[i] 
        );
    }
    delete rManage;
}

void ASTInterpreter::printCallIndent() {
    std::string indent = "";
    for (int i = 0 ; i < this->callStack.size() ; i++) {
        indent.push_back('-');
    }
    if (indent.size() != 0) {
        std::cout << indent << " ";
    } 
}