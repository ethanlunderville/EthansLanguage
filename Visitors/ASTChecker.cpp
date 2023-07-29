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
    typeManager->createType(sDecTree->getIdentifier(), new Struct(this->contextManager->popScope(true)));
    //this->contextManager->declareSymbol(sDecTree->getLine(), sDecTree->getIdentifier(), typeManager->getTypeHandler(sDecTree->getIdentifier()));
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
    ((Evaluatable*)astree)->setVal(structTable->getValueStoredInSymbol(iDent->getIdentifier()));
}
void ASTChecker::visitAssignOpTree (AST* astree) {
    AssignOpTree* tree = ((AssignOpTree*)astree);
    Type* type = contextManager->getTypeOfSymbol(dynamic_cast<IdentifierTree*>(tree->getChildren()[0])->getIdentifier());
    Assignable* assignableType = dynamic_cast<Assignable*>(tree->getChildren()[1]);
    assignableType->setType(this->contextManager->getTypeOfSymbol(dynamic_cast<IdentifierTree*>(tree->getChildren()[0])->getIdentifier()));
    assignableType->setCheckerReference(this);
    type->checkAssignment(assignableType);
    this->visitChildren(astree);
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
void ASTChecker::visitAssignTree(AST* astree) {}
void ASTChecker::visitFunctionAssignTree(AST* astree) {
    this->clearCurrentFunctionReturns();
    this->visitChildren(astree);
    Type* functionType = dynamic_cast<FunctionAssignTree*>(astree)->getType();
    for (int i = 0 ; i < this->returnTrees.size() ; i++) {
        if (this->returnTrees[i]->getChildren().size() > 0) {
            ExpressionTree* eTree = dynamic_cast<ExpressionTree*>(this->returnTrees[i]->getChildren()[i]);
            functionType->checkType(eTree->getVal());
        }
    }
    this->clearCurrentFunctionReturns();

}
void ASTChecker::visitArrayAssignTree(AST* astree) {}
void ASTChecker::visitStructAssignTree(AST* astree) {}
/*****************************/
void ASTChecker::visitReturnTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitIfTree(AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitProgramTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitBlockTree (AST* astree) {this->visitChildren(astree);}
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

void ASTChecker::addCurrentFunctionReturnTree(ReturnTree* rTree) {
    this->returnTrees.push_back(rTree);
}

void ASTChecker::clearCurrentFunctionReturns() {
    if (this->returnTrees.size() > 0) {
        this->returnTrees.clear();
    }
}