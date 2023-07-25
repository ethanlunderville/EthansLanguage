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
void ASTChecker::visitAssignTree(AST* astree) {
    //AssignTree* tree = ((AssignTree*)astree);
    //Type* type = contextManager->getTypeOfSymbol(dynamic_cast<IdentifierTree*>(tree->getChildren()[0]->getIdentifier()));
    //try to downcast to a Primitive type and only check if cast succeeds
    //if (type != nullptr) {
    //    if (!(type->changeExpressionToDeclaredTypeIfItIsLegal(tree->getChildren()[0], tree->getLine(), this->contextManager))) {
    //        exit(1);    
    //    }
    //} else {
    //    std::cerr << "Compiler error:: expected a primitive type but got a complex type. Error occured on line: " << tree->getLine() << std::endl; 
    //}  
}

void ASTChecker::visitFunctionAssignTree(AST* astree) {

}
void ASTChecker::visitArrayAssignTree(AST* astree) {}
void ASTChecker::visitStructAssignTree(AST* astree) {}
void ASTChecker::visitIfTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitExpressionTree (AST* astree) {
    this->visitChildren(astree);
}
void ASTChecker::visitDivideTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitMultiplyTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitAddTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitSubtractTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitExponentTree (AST* astree) {this->visitChildren(astree);}
// Primitive declaration
void ASTChecker::visitDeclarationTree (AST* astree) {
    DeclarationTree* t = ((DeclarationTree*)astree);
    Type* type = this->typeManager->getTypeHandler(t->getType());
    this->contextManager->declareSymbol(t->getLine(), t->getIdentifier(), type);
    contextManager->printSymbolTable();
    if (t->getChildren().size() != 0) {
        this->visitChildren(astree);
    }
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
    this->contextManager->declareSymbol(sDecTree->getLine(), sDecTree->getIdentifier(), typeManager->getTypeHandler(sDecTree->getIdentifier()));
}

void ASTChecker::visitBlockTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitReturnTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitProgramTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitFunctionCallTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitWhileTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitElseTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitNumberTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitStringTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitIdentifierTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitGreaterTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitGreaterEqualTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitLessTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitLessEqualTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitEqualTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitNotEqualTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitAndTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitOrTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitArrowOpTree (AST* astree) {this->visitChildren(astree);}
void ASTChecker::visitAssignOpTree (AST* astree) {
    AssignOpTree* tree = ((AssignOpTree*)astree);
    Type* type = contextManager->getTypeOfSymbol(dynamic_cast<IdentifierTree*>(tree->getChildren()[0])->getIdentifier());
    Assignable* assignableType = dynamic_cast<Assignable*>(tree->getChildren()[1]);
    assignableType->setCheckerReference(this->contextManager);
    type->checkAssignment(assignableType);
}
void ASTChecker::visitPrintTree (AST* astree) {this->visitChildren(astree);}
