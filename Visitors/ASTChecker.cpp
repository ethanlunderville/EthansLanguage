#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include "SymbolTable/ContextManager.h"

ASTChecker::ASTChecker(TypeManager* typeManager) {
    this->contextManager = new ContextManager(typeManager);
    this->typeManager = typeManager;
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

void ASTChecker::visitBlockTree (AST* astree) {
    this->contextManager->pushScope();
    this->visitChildren(astree);
    this->contextManager->popScope(false);
}

void ASTChecker::visitExpressionTree (AST* astree) {
    this->visitChildren(astree);
    Type* eType = dynamic_cast<ExpressionTree*>(astree)->getType();
    if (eType != nullptr) {
        if (!eType->checkType(dynamic_cast<Evaluatable*>(astree->getChildren()[0])->getVal())) {
            std::cerr << "Attempted to assign incorrect value to variable of type: ";
            eType->printType();
            std::cerr << "" << std::endl;
            exit(1);
        }
        return;
    }
    ((ExpressionTree*)astree)->setVal(dynamic_cast<Evaluatable*>(astree->getChildren()[0])->getVal());
}

void ASTChecker::visitFunctionCallTree (AST* astree) {
    this->visitChildren(astree);
    FunctionCallTree* fCall = dynamic_cast<FunctionCallTree*>(astree);
    fCall->setVal(this->contextManager->getValueStoredInSymbol(fCall->getIdentifier()));
}
// Primitive declaration
void ASTChecker::visitDeclarationTree (AST* astree) {
    DeclarationTree* t = ((DeclarationTree*)astree);
    Type* type = this->typeManager->getTypeHandler(t->getType());
    this->contextManager->declareSymbol(t->getLine(), t->getIdentifier(), type);
    this->visitChildren(astree);
    contextManager->printSymbolTable();
    return;
}
void ASTChecker::visitArrayDeclarationTree (AST* astree) {
    ArrayDeclarationTree* arrayTree = ((ArrayDeclarationTree*)astree);
    Type* primTypeReference = this->typeManager->getTypeHandler(arrayTree->getType());
    this->contextManager->declareSymbol(arrayTree->getLine(), arrayTree->getIdentifier(), new Array(primTypeReference));
    contextManager->printSymbolTable();
    this->visitChildren(astree);
    return;
}
void ASTChecker::visitFunctionDeclarationTree (AST* astree) {
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
void ASTChecker::visitStructDeclarationTree (AST* astree) {
    StructDeclarationTree* sDecTree = ((StructDeclarationTree*)astree);
    this->contextManager->pushScope();
    // BYPASS THE BLOCK TREE VISIT BY VISITING ITS CHILDREN DIRECTLY TO AVOID POPPING SCOPE
    this->visitChildren(sDecTree->getChildren()[0]);
    typeManager->createType(
        sDecTree->getIdentifier(),
        new Struct(this->contextManager->popScope(true),
        sDecTree->getIdentifier())
    );
}
void ASTChecker::visitAddTree (AST* astree) {
    this->visitChildren(astree);
    int i = 0;
    while (i<2) {
        std::any current = dynamic_cast<Evaluatable*>(astree->getChildren()[i])->getVal();
        if (current.type() == typeid(std::string)) {
            ((AddTree*)astree)->setVal(current);
            return;
        } else if (current.type() == typeid(double)) {
            ((AddTree*)astree)->setVal(current);
        }
        i++;
    }
}
void ASTChecker::visitDivideTree (AST* astree) {
    this->visitChildren(astree);
    this->numberCheck(astree);
    ((Evaluatable*)astree)->setVal(1.00);
}
void ASTChecker::visitMultiplyTree (AST* astree) {
    this->visitChildren(astree);
    this->numberCheck(astree);
    ((Evaluatable*)astree)->setVal(1.00);
}
void ASTChecker::visitSubtractTree (AST* astree) {
    this->visitChildren(astree);
    this->numberCheck(astree);
    ((Evaluatable*)astree)->setVal(1.00);
}
void ASTChecker::visitExponentTree (AST* astree) {
    this->visitChildren(astree);
    this->numberCheck(astree);
    ((Evaluatable*)astree)->setVal(1.00);
}
void ASTChecker::visitGreaterTree (AST* astree) {
    this->visitChildren(astree);
    this->numberCheck(astree);
    ((Evaluatable*)astree)->setVal(1.00);
}
void ASTChecker::visitGreaterEqualTree (AST* astree) {
    this->visitChildren(astree);
    this->numberCheck(astree);
    ((Evaluatable*)astree)->setVal(1.00);
}
void ASTChecker::visitLessTree (AST* astree) {
    this->visitChildren(astree);
    this->numberCheck(astree);
    ((Evaluatable*)astree)->setVal(1.00);
}
void ASTChecker::visitLessEqualTree (AST* astree) {
    this->visitChildren(astree);
    this->numberCheck(astree);
    ((Evaluatable*)astree)->setVal(1.00);
}
void ASTChecker::visitEqualTree (AST* astree) {
    this->visitChildren(astree);
    this->numberCheck(astree);
    ((Evaluatable*)astree)->setVal(1.00);    
}
void ASTChecker::visitNotEqualTree (AST* astree) {
    this->visitChildren(astree);
    this->numberCheck(astree);
    ((Evaluatable*)astree)->setVal(1.00);
}
void ASTChecker::visitAndTree (AST* astree) {
    this->visitChildren(astree);
    this->numberCheck(astree);
    ((Evaluatable*)astree)->setVal(1.00);
}
void ASTChecker::visitOrTree (AST* astree) {
    this->visitChildren(astree);
    this->numberCheck(astree);
    ((Evaluatable*)astree)->setVal(1.00);
}
/*BUBBLE BASE VALUE TO THE TOP*/
void ASTChecker::visitArrowOpTree (AST* astree) {
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
            if (typeid(*iDent) == typeid(ArrayAccessTree)) { /*ARRAY ACCESS IDENT*/
                Array* arrayType = dynamic_cast<Array*>(structTable->getTypeOfSymbol(iDent->getIdentifier()));
                if (typeid(*(arrayType->getArrayType())) == typeid(Struct)) { /* ARRAY TYPE RETURNS STRUCT */
                    Struct* sType = dynamic_cast<Struct*>(arrayType->getArrayType());
                    dynamic_cast<ArrowOpTree*>(astree)->setVal(sType->getBaseStructPointer());
                    return;
                }
                dynamic_cast<ArrowOpTree*>(astree)->setVal(arrayType->getArrayType()->getNullValue());
                return;
            }
            if (typeid(*iDent) == typeid(FunctionCallTree)) { /*FUNCTION CALL IDENT*/
                Function* functionType = dynamic_cast<Function*>(structTable->getTypeOfSymbol(iDent->getIdentifier()));
                if (typeid(*(functionType->getFunctionType())) == typeid(Struct)) { /* FUNCTION CALL RETURNS STRUCT */
                    Struct* sType = dynamic_cast<Struct*>(functionType->getFunctionType());
                    dynamic_cast<ArrowOpTree*>(astree)->setVal(sType->getBaseStructPointer());
                    return;
                }
                dynamic_cast<ArrowOpTree*>(astree)->setVal(functionType->getFunctionType()->getNullValue());
                return;
            }
            if (typeid(*(structTable->getTypeOfSymbol(iDent->getIdentifier()))) == typeid(Struct)) { /*STRUCT IDENT*/
                Struct* sType = dynamic_cast<Struct*>(structTable->getTypeOfSymbol(iDent->getIdentifier()));
                iDent->setVal(sType->getBaseStructPointer());
                return;
            }
            std::any temp = structTable->getValueStoredInSymbol(iDent->getIdentifier()); /*PRIMITIVE IDENT*/
            dynamic_cast<Evaluatable*>(astree)->setVal(temp);
            return;
        } else {
            std::cerr << "Incorrect arrow expression" << std::endl;
            exit(1);
        }
    }
}

/*IF THE TYPE IS AN ARROW OP TREE THEN BUBBLE THE TYPE TO THE TOP*/
void ASTChecker::visitLValArrowOpTree(AST* astree) {
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
        if (iDent != nullptr) { 
            Type* temp;
            temp = structTable->getTypeOfSymbol(iDent->getIdentifier()); /*BASIC LVAL IDENTIFIER*/
            if (typeid(*iDent) == typeid(ArrayAccessTree)) { /*IF THE IDENTIFIER IS AN ARRAY ELEMENT*/
                temp = dynamic_cast<Array*>(temp)->getArrayType();
            } else if (temp == nullptr) {
                std::cerr << "Unrecognized identifier :: " << iDent->getIdentifier() << std::endl;
                exit(1);
            }
            dynamic_cast<Evaluatable*>(astree)->setVal(temp);
            return;
        } else {
            std::cerr << "Incorrect arrow expression" << std::endl;
            exit(1);
        }
    }
}

void ASTChecker::visitLValIdentifierTree (AST* astree) {
    IdentifierTree* iDent = ((IdentifierTree*)astree);
    Type* type = this->contextManager->getTypeOfSymbol(iDent->getIdentifier());
    iDent->setVal(type);
}

void ASTChecker::visitIdentifierTree (AST* astree) {
    this->visitChildren(astree);
    IdentifierTree* identer = ((IdentifierTree*)astree);
    if (identer->LVal) { return this->visitLValIdentifierTree(astree); }
    if ( typeid(*(this->contextManager->getTypeOfSymbol(identer->getIdentifier()))) == typeid(Struct) ) { // IF IDENT IS A STRUCT
        Struct* sType = dynamic_cast<Struct*>(this->contextManager->getTypeOfSymbol(identer->getIdentifier()));
        identer->setVal(sType->getBaseStructPointer());
        return;
    } 
    identer->setVal(this->contextManager->getValueStoredInSymbol(identer->getIdentifier()));
}

void ASTChecker::visitLValArrayAccessTree(AST* astree) {
    ArrayAccessTree* aAccess = dynamic_cast<ArrayAccessTree*>(astree);
    Type* aType = this->contextManager->getTypeOfSymbol(aAccess->getIdentifier());
    aAccess->setVal(dynamic_cast<Array*>(aType)->getArrayType());
}

void ASTChecker::visitArrayAccessTree(AST* astree) {
    ArrayAccessTree* aAccess = dynamic_cast<ArrayAccessTree*>(astree);
    if (aAccess->LVal) { return this->visitLValArrayAccessTree(aAccess); }
    Type* t = this->contextManager->getTypeOfSymbol(aAccess->getIdentifier());
    Array* aType = dynamic_cast<Array*>(t);
    if (typeid(*(aType->getArrayType())) == typeid(Struct) ) {
        Struct* sType = dynamic_cast<Struct*>(aType->getArrayType());
        aAccess->setVal(sType->getBaseStructPointer());
        return;
    }
    aAccess->setVal(aType->getArrayType()->getNullValue());
}

void ASTChecker::visitAssignOpTree (AST* astree) {
    AssignOpTree* tree = ((AssignOpTree*)astree);
    Assignable* assignableType = dynamic_cast<Assignable*>(tree->getChildren()[1]);
    Evaluatable* lValTree = dynamic_cast<Evaluatable*>(tree->getChildren()[0]);
    lValTree->LVal = true;
    lValTree->accept(this);
    assignableType->setType(std::any_cast<Type*>(lValTree->getVal()));
    assignableType->accept(this);
    contextManager->printSymbolTable();
}

void ASTChecker::visitNumberTree (AST* astree) {
    this->visitChildren(astree);
    double temp = 0.00;
    ((NumberTree*)astree)->setVal(temp);
}

void ASTChecker::visitStringTree (AST* astree) {
    this->visitChildren(astree);
    ((StringTree*)astree)->setVal(std::string(""));
}

/*HANDLED IN SPECIFIC CLASSES*/
void ASTChecker::visitFunctionAssignTree(AST* astree) {/**/
    FunctionAssignTree* fAssignTree = dynamic_cast<FunctionAssignTree*>(astree);
    this->contextManager->pushContext();
    this->contextManager->setCurrentFunctionType(dynamic_cast<Function*>(fAssignTree->getType())->getFunctionType());
    this->visitChildren(astree);
    this->contextManager->popContext();
}

void ASTChecker::visitReturnTree (AST* astree) {
    if (astree->getChildren().size() > 0) {
        dynamic_cast<ExpressionTree*>(astree->getChildren()[0])
        ->setType(this->contextManager->getCurrentFunctionType());
        this->visitChildren(astree);
        return;
    }
    std::cerr << "Functions must return at least one value" << std::endl;
    exit(1);
}


void ASTChecker::visitArrayAssignTree(AST* astree) {
    Type* aType = dynamic_cast<ArrayAssignTree*>(astree)->getType(); 
    if (typeid(*aType) != typeid(Array)) {
        std::cerr << "Incorrect assignment, cannot assign array to non array type" << std::endl;
        exit(1);
    } else {
        aType = dynamic_cast<Array*>(aType)->getArrayType();
    }
    std::vector<AST*>& kids = astree->getChildren();
    for (AST* potentiallyValidChild : kids) {
        ExpressionTree* child = dynamic_cast<ExpressionTree*>(potentiallyValidChild);
        if (child == nullptr) {
            std::cerr << "COMPILER ERROR::ALL ELEMENTS IN AN ARRAY ASSIGNMENT MUST BE EXPRESSION NODES" << std::endl;
        }
        this->visitExpressionTree(child);
        std::any val = child->getVal();
        if (!aType->checkType(val)) {
            std::cerr << "Invlaid array assignement due to attempting to assign an array with at least one element of an incorrect type " << std::endl;
            exit(1);
        }
    }
}
void ASTChecker::visitStructAssignTree(AST* astree) {
    StructAssignTree* sAssignTree = dynamic_cast<StructAssignTree*>(astree);
    std::string typeName = sAssignTree->getTypeName();
    if (sAssignTree->getType() != this->typeManager->getTypeHandler(typeName)) {
        std::cerr << "Incorred type assignment for type " << typeName << std::endl;
        exit(1);
    }
}
/*****************************/

void ASTChecker::visitIfTree(AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitProgramTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitWhileTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitElseTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitPrintTree (AST* astree) {this->visitChildren(astree);}


bool ASTChecker::numberCheck(AST* astree) {
    int i = 0;
    while (i < 2) {
        Evaluatable* eval = dynamic_cast<Evaluatable*>(astree->getChildren()[i]);
        if (eval == nullptr) {
            std::cerr << "Expected node" << astree << "to have evaluatable child" << std::endl;
        } else if (eval->getVal().type() != typeid(double)) {
            std::cerr << "Expected child to be of type double" << std::endl;
            exit(1);
        }
        i++;
    }
    return true;
}