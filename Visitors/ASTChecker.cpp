#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include "SymbolTable/ContextManager.h"

/*

    IMPORTANT 

    Please take not that all nodes that can appear as a 
    left child of an assignment operation have another 
    visitLValue visitor function associate with them. 
    As of right now the only types that can be found
    as a left child of an assign op tree are 

    1. ArrayAccessTrees :: For assigning array elements 
    2. IdentifierTrees :: For assigning values of identifier 
    3. ArrowOpTrees :: For assigning struct elements i.e "structname->element"

    No assignments take place in this phase. Instead the 
    AssignOpTree visitor function bubbles the l-value 
    trees type up and assigns it the the value field 
    in the l-value tree node (all l-val trees inherit 
    Evaluatable and there for have a value field)

    this value is assigned to the type field in the r-value
    node (All r-value nodes are decendants of the Assignable
    class which means they all have a type field). Using this 
    type the value of the r-value node is checked to see if
    it matches the tpye of the l-value

    NOTE: Structs are still very unstable

*/

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
    dynamic_cast<ExpressionTree*>(astree)->setVal(
        dynamic_cast<Evaluatable*>(astree->getChildren()[0])->getVal()
    );
}

void ASTChecker::visitFunctionCallTree (AST* astree) {
    this->visitChildren(astree); 
    FunctionCallTree* fCall = dynamic_cast<FunctionCallTree*>(astree);
    if (Builtins::builtInFunctions[fCall->getIdentifier()] != nullptr) {
        fCall->setVal(Builtins::baseValues[fCall->getIdentifier()]);
        return;
    }
    fCall->setVal(this->contextManager->getValueStoredInSymbol(fCall->getIdentifier()));
}

void ASTChecker::visitDeclarationTree (AST* astree) {
    DeclarationTree* t = ((DeclarationTree*)astree);
    Type* type = this->typeManager->getTypeHandler(t->getType());
    this->contextManager->declareSymbol(t->getLine(), t->getIdentifier(), type);
    this->visitChildren(astree);
    #ifdef PRINTSYMBOLS
        contextManager->printSymbolTable();
    #endif
    return;
}

void ASTChecker::visitArrayDeclarationTree (AST* astree) {
    ArrayDeclarationTree* arrayTree = ((ArrayDeclarationTree*)astree);
    Type* primTypeReference = this->typeManager->getTypeHandler(arrayTree->getType());
    this->contextManager->declareSymbol(
        arrayTree->getLine(),
        arrayTree->getIdentifier(), 
        new Array(primTypeReference)
    );
    this->visitChildren(astree);
    #ifdef PRINTSYMBOLS
        contextManager->printSymbolTable();
    #endif
    return;
}

void ASTChecker::visitFunctionDeclarationTree (AST* astree) {
    FunctionDeclarationTree* functionTree = ((FunctionDeclarationTree*)astree);
    Type* primTypeReference = this->typeManager->getTypeHandler(functionTree->getType());
    this->contextManager->declareSymbol(
        functionTree->getLine(), 
        functionTree->getIdentifier(), 
        new Function(primTypeReference)
    );
    this->visitChildren(astree);
    if (functionTree->getChildren().size() != 0) {
        this->visitChildren(astree);
    }
    #ifdef PRINTSYMBOLS
        contextManager->printSymbolTable();
    #endif
    return;
}

void ASTChecker::visitStructDeclarationTree (AST* astree) {
    StructDeclarationTree* sDecTree = ((StructDeclarationTree*)astree);
    Struct* newStruct = new Struct(sDecTree->getIdentifier());
    typeManager->createType(sDecTree->getIdentifier(), newStruct);
    std::vector<AST*>& blockKids = sDecTree->getChildren()[0]->getChildren();
    for (int i = 0 ; i < blockKids.size() ;i++) {
        if (dynamic_cast<Declarable*>(blockKids[i]) == nullptr) { //if the node is not a declaration
            if (typeid(*blockKids[i]) == typeid(ExpressionTree)) { // and if it is an expression
                std::vector<AST*>& expressionKids = blockKids[i]->getChildren();
                if (typeid(*expressionKids[0]) != typeid(AssignOpTree) 
                || typeid(*expressionKids[0]->getChildren()[1]) != typeid(FunctionAssignTree)) { // if the assignment is not a functions
                    std::cerr << "Only functions can be assigned within the scope of a struct" << std::endl;
                    exit(1);
                }
            } else {
                std::cerr << "Only declaractions and function assignments may be present in struct declaration." << std::endl;
                exit(1);
            }
        }
    } 
    this->contextManager->pushScope();
    this->contextManager->declareSymbol(0, std::string("this"), newStruct);
    this->contextManager->reassignSymbol(std::string("this"), newStruct->getBaseStructPointer(), 0);
    this->visitChildren(sDecTree->getChildren()[0]);
    SymbolTable* baseStruct = this->contextManager->popScope(true);
    baseStruct->declareSymbol(0, std::string("this"), newStruct);
    baseStruct->reassignSymbol(std::string("this"), baseStruct);
    newStruct->setBaseStructPointer(baseStruct);
}

void ASTChecker::visitAddTree (AST* astree) {
    this->visitChildren(astree);
    std::any setVal;
    int i = 0;
    while (i < 2) {
        std::any current = dynamic_cast<Evaluatable*>(astree->getChildren()[i])->getVal();
        if (current.type() == typeid(std::vector<std::any>)) {
            dynamic_cast<AddTree*>(astree)->setVal(current);
            return;
        } else if (current.type() == typeid(std::string)) {
            if (setVal.type() != typeid(std::vector<std::any>)) {
                dynamic_cast<AddTree*>(astree)->setVal(current);
                setVal = current;
            }
        } else if (current.type() == typeid(double)) {
            if (setVal.type() != typeid(std::string) && setVal.type() != typeid(std::vector<std::any>)) {
                dynamic_cast<AddTree*>(astree)->setVal(current);
                setVal = current;
            }
        }
        i++;
    }
    if (!setVal.has_value()) {
        std::cerr << "Incorrect addition value" << std::endl;
        exit(1);
    }
    dynamic_cast<AddTree*>(astree)->setVal(setVal);
}

/*BUBBLE BASE VALUE TO THE TOP*/
void ASTChecker::visitArrowOpTree (AST* astree) {
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

/* BUBBLE THE TYPE TO THE TOP */
void ASTChecker::visitLValArrowOpTree(AST* astree) {
    /*IF SOMETHING IS ON THE LEFT OF AN ARROW IT MUST BE IDENTIFIABLE */
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
            if (iDent->getIdentifier().compare("this") == 0) {
                dynamic_cast<Evaluatable*>(astree)->setVal(this->typeManager->getTypeHandler(structTable->getTypeName()));
                return;
            }
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
    if (identer->getIsLVal()) { return this->visitLValIdentifierTree(astree); }
    if ( typeid(*(this->contextManager->getTypeOfSymbol(identer->getIdentifier()))) == typeid(Struct) ) { // IF IDENT IS A STRUCT
        Struct* sType = dynamic_cast<Struct*>(
            this->contextManager->getTypeOfSymbol(identer->getIdentifier())
        );
        identer->setVal(sType->getBaseStructPointer());
        return;
    } 
    identer->setVal(this->contextManager->getValueStoredInSymbol(identer->getIdentifier()));
}

void ASTChecker::visitLValArrayAccessTree(AST* astree) {
    ArrayAccessTree* aAccess = dynamic_cast<ArrayAccessTree*>(astree);
    Type* aType = this->contextManager->getTypeOfSymbol(aAccess->getIdentifier());
    if (typeid(*aType) == typeid(String)) {
        aAccess->setVal(aType);
        return;
    }
    aAccess->setVal(dynamic_cast<Array*>(aType)->getArrayType());
}

void ASTChecker::visitArrayAccessTree(AST* astree) {
    ArrayAccessTree* aAccess = dynamic_cast<ArrayAccessTree*>(astree);
    if (aAccess->getIsLVal()) { return this->visitLValArrayAccessTree(aAccess); }
    Type* t = this->contextManager->getTypeOfSymbol(aAccess->getIdentifier());
    if (typeid(*t) == typeid(Array)) {
        Array* aType = dynamic_cast<Array*>(t);
        if (typeid(*(aType->getArrayType())) == typeid(Struct) ) {
            Struct* sType = dynamic_cast<Struct*>(aType->getArrayType());
            aAccess->setVal(sType->getBaseStructPointer());
            return;
        }
        aAccess->setVal(aType->getArrayType()->getNullValue());
    } else if (typeid(*t) == typeid(String)) {
        aAccess->setVal(std::any(""));
        return;
    }
}

void ASTChecker::visitAssignOpTree (AST* astree) {
    AssignOpTree* tree = ((AssignOpTree*)astree);
    Assignable* assignableType = dynamic_cast<Assignable*>(tree->getChildren()[1]);
    Evaluatable* lValTree = dynamic_cast<Evaluatable*>(tree->getChildren()[0]);
    lValTree->setIsLVal(true);
    lValTree->accept(this);
    assignableType->setType(std::any_cast<Type*>(lValTree->getVal()));
    assignableType->accept(this);
    if (typeid(*assignableType) == typeid(FunctionAssignTree)) {
        ASTInterpreter* aI = new ASTInterpreter(this->typeManager, this->contextManager);
        aI->temporary = true;
        astree->accept(aI);
        delete aI;
    }
    #ifdef PRINTSYMBOLS
        contextManager->printSymbolTable();
    #endif
}

void ASTChecker::visitFunctionAssignTree(AST* astree) {/**/
    FunctionAssignTree* fAssignTree = dynamic_cast<FunctionAssignTree*>(astree);
    this->contextManager->pushContext();
    this->contextManager->setCurrentFunctionType(
        dynamic_cast<Function*>(fAssignTree->getType())->getFunctionType()
    );
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
    ((Evaluatable*)astree)->setVal(1.00);    
}
void ASTChecker::visitNotEqualTree (AST* astree) {
    this->visitChildren(astree);
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

void ASTChecker::visitNumberTree (AST* astree) {
    NumberTree* nTree = dynamic_cast<NumberTree*>(astree);
    nTree->setVal(nTree->getNumber());
}

void ASTChecker::visitStringTree (AST* astree) {
    StringTree* sTree = dynamic_cast<StringTree*>(astree);
    sTree->setVal(sTree->getString());
}


void ASTChecker::visitIfTree(AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitProgramTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitWhileTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitElseTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitRegexSectionTree (AST* astree) {/*this->visitChildren(astree);*/}