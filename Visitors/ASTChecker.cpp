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

void ASTChecker::visitExpressionTree (AST* astree) {
    this->visitChildren(astree);
    Type* eType = dynamic_cast<ExpressionTree*>(astree)->getType();
    if (eType != nullptr) {
        if (
            !eType->checkType(dynamic_cast<Evaluatable*>(astree->getChildren()[0])->getVal())
            ) 
            {
            std::cerr << "Attempted to assign incorrect value to variable of type: ";
            eType->printType();
            std::cerr << "" << std::endl;
            exit(1);
        }
        return;
    }
    ((ExpressionTree*)astree)->setVal(dynamic_cast<Evaluatable*>(astree->getChildren()[0])->getVal());
}

void ASTChecker::visitIdentifierTree (AST* astree) {
    this->visitChildren(astree);
    IdentifierTree* identer = ((IdentifierTree*)astree);
    identer->setVal(this->contextManager->getValueStoredInSymbol(identer->getIdentifier()));
}
void ASTChecker::visitFunctionCallTree (AST* astree) {
    this->visitChildren(astree);
    FunctionCallTree* identer = ((FunctionCallTree*)astree);
    identer->setVal(this->contextManager->getValueStoredInSymbol(identer->getIdentifier()));
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
    contextManager->printSymbolTable();
    if (functionTree->getChildren().size() != 0) {
        this->visitChildren(astree);
    }
    contextManager->printSymbolTable();
    return;
}
void ASTChecker::visitStructDeclarationTree (AST* astree) {
    StructDeclarationTree* sDecTree = ((StructDeclarationTree*)astree);
    this->contextManager->pushScope();
    this->visitChildren(sDecTree->getChildren()[0]);
    typeManager->createType(sDecTree->getIdentifier(), new Struct(this->contextManager->popScope(true), sDecTree->getIdentifier()));
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
void ASTChecker::visitArrowOpTree (AST* astree) {
    Identifiable* eValType = dynamic_cast<Identifiable*>(astree->getChildren()[0]);
    if (eValType == nullptr) {
        std::cerr << "Non struct identifier with arrow following it" << std::endl;
        exit(1);
    } else if (
        typeid(*eValType) == typeid(IdentifierTree) 
        && eValType->getChildren().size() == 1 
        && typeid(*(eValType->getChildren()[0])) == typeid(ExpressionTree)
       ) {
        this->visitIdentifierTree(dynamic_cast<IdentifierTree*>(eValType));
    }
    std::any structAny;
    if (this->structScoper.size() > 0) {
        structAny = this->structScoper.top()->getValueStoredInSymbol(eValType->getIdentifier());
    } else {
        structAny = this->contextManager->getValueStoredInSymbol(eValType->getIdentifier());
    }  
    SymbolTable* structTable = std::any_cast<SymbolTable*>(structAny);

    this->structScoper.push(structTable);
    this->visitChildren(astree->getChildren()[1]);
    this->structScoper.pop();

    Identifiable* iDent = dynamic_cast<Identifiable*>(eValType->getChildren()[1]);
    ArrowOpTree* aTree = dynamic_cast<ArrowOpTree*>(astree);
    aTree->setVal(structTable->getValueStoredInSymbol(iDent->getIdentifier()));
    aTree->setType(structTable->getTypeOfSymbol(iDent->getIdentifier()));
}
void ASTChecker::visitAssignOpTree (AST* astree) {
    AssignOpTree* tree = ((AssignOpTree*)astree);
    Assignable* assignableType = dynamic_cast<Assignable*>(tree->getChildren()[1]);
    AST* lValTree = tree->getChildren()[0];
    Type* type;
    if (typeid(*lValTree) == typeid(IdentifierTree)) {
        IdentifierTree* iTree = dynamic_cast<IdentifierTree*>(lValTree);
        type = contextManager->getTypeOfSymbol(iTree->getIdentifier());
        if (lValTree->getChildren().size() > 0 && typeid(*(lValTree->getChildren()[0])) == typeid(ExpressionTree)) {
            type = dynamic_cast<Array*>(type)->getArrayType();
        }
        assignableType->setType(type);
        this->visitChildren(astree);
        /* Figure out if struct is copied or declared. If copied iterate the refcount */
        if (typeid(*type) == typeid(Struct)) {
            if (typeid(*assignableType) == typeid(ExpressionTree)) {
                SymbolTable* structRef = std::any_cast<SymbolTable*>(
                    dynamic_cast<Evaluatable*>(
                        assignableType->getChildren()[0]
                    )->getVal()
                ); 
                structRef->incrementReferenceCount();
                this->contextManager->reassignSymbol(
                    iTree->getIdentifier(),
                    structRef,
                    assignableType->getLine()
                );
            } else if (typeid(*assignableType) == typeid(StructAssignTree)) {
                Type* structType = dynamic_cast<Struct*>(type);
                SymbolTable* baseSymbolTable = structType->getDuplicateBase();
                this->contextManager->reassignSymbol(
                    iTree->getIdentifier(),
                    baseSymbolTable,
                    assignableType->getLine()
                );
                /*THIS KEYWORD*/
                baseSymbolTable->declareSymbol(assignableType->getLine(), "this", structType);
                baseSymbolTable->reassignSymbol("this", baseSymbolTable);
            }
        }
    } else if (typeid(*lValTree) == typeid(ArrowOpTree)) {
        this->visitArrowOpTree(lValTree);
        type = dynamic_cast<ArrowOpTree*>(lValTree)->getType();
    }
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

void ASTChecker::visitBlockTree (AST* astree) {
    this->contextManager->pushScope();
    this->visitChildren(astree);
    this->contextManager->popScope(false);
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
void ASTChecker::visitAssignTree(AST* astree) {/*REMOVE*/}

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