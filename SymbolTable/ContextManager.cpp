#include "ContextManager.h"

ContextManager::ContextManager(TypeManager* typeManager) {
    this->globalContext = new SymbolTable(nullptr);
    this->typeManager = typeManager;
}
ContextManager::~ContextManager() {
    while (this->contextStack.size() != 0) {
        delete contextStack.top();
        contextStack.pop();
    }
    delete this->globalContext;
    this->globalContext = nullptr; 
    delete this->typeManager;
    this->typeManager = nullptr;
}
void ContextManager::pushContext() {
    this->contextStack.push(new SymbolTable(this->globalContext));
}
void ContextManager::popContext() {
    if (contextStack.size() > 0) {
        delete contextStack.top();
        contextStack.pop();
        return;
    }
    std::cerr << "Unable to pop empty context stack" << std::endl;
    exit(1);
}
void ContextManager::pushScope() {
    if (contextStack.size() > 0) {
        contextStack.top()->pushScope();
        return;
    } 
    std::cerr << "No scopes allowed in global memory" << std::endl; 
    exit(1);
}
void ContextManager::popScope(){
    if (contextStack.size() > 0) {
        contextStack.top()->popScope();
        return;
    } 
    std::cerr << "Unable to pop empty scope stack" << std::endl; 
    exit(1);
}
void ContextManager::declareSymbol(int line, std::string identifier, std::string type) {
    Type* currentType = this->typeManager->getTypeHandler(type);  
    if (this->contextStack.size() != 0) {
        if (this->contextStack.top()->contains(identifier) == -1 && this->globalContext->contains(identifier) == -1) {
            this->contextStack.top()->declareSymbol(line, identifier, currentType);
        } else {
            std::cerr << 
            "Incorrect declaration of already declared variable: " 
            << identifier  << " on line: " << line 
            << std::endl;
            exit(1);
        } 
    } else { // IF WE ARE IN GLOBAL SCOPE
        if (this->globalContext->contains(identifier) == -1) {
            this->globalContext->declareSymbol(line, identifier, currentType);
        } else {
            std::cerr << 
            "Incorrect declaration of already declared variable: " 
            << identifier  << " on line: " << line 
            << std::endl;
            exit(1);
        }
    }
}   
void ContextManager::reassignSymbol(std::string identifier , std::any value, int line) { 
    if (this->contextStack.size() != 0 && this->contextStack.top()->contains(identifier) != -1) {
        if (this->contextStack.top()->variableTypeCheck(this->contextStack.top()->getTypeOfSymbol(identifier), value)) {
            this->contextStack.top()->reassignSymbol(identifier, value);
            return;
        } else {
            goto INCORRECTTYPEERROR;
        }
    }
    if (this->globalContext->contains(identifier) != -1) {
        if (this->globalContext->variableTypeCheck(this->globalContext->getTypeOfSymbol(identifier), value)) {
            this->globalContext->reassignSymbol(identifier, value);
            return;
        } else {
            goto INCORRECTTYPEERROR;
        }
    }
    std::cerr <<
    "Incorrect assignment of already undeclared variable: " 
    << identifier  << " on line: " << line 
    << std::endl;
    exit(1);
INCORRECTTYPEERROR:
    std::cerr << 
    "Incorrect type assignment for variable: " 
    << identifier << " on line: " << line
    << std::endl;
    exit(1);
}   
std::any ContextManager::getValueStoredInSymbol(std::string identifier) {
    if (this->contextStack.size() != 0 && this->contextStack.top()->contains(identifier) == -1) {
        return this->contextStack.top()->getValueStoredInSymbol(identifier);
    } else if (this->globalContext->contains(identifier) == -1) {
        return this->globalContext->getValueStoredInSymbol(identifier);
    } else {
        std::cerr << "Unrecognized identifier: " << identifier << std::endl;
        exit(1);
    }
}
Type* ContextManager::getTypeOfSymbol(std::string identifier) {
    if (this->contextStack.size() != 0 && this->contextStack.top()->contains(identifier) != -1) {
        return this->contextStack.top()->getTypeOfSymbol(identifier);
    } else if (this->globalContext->contains(identifier) != -1) {
        return this->globalContext->getTypeOfSymbol(identifier);
    } 
    std::cerr << "Unrecognized identifier: " << identifier <<std::endl;
    exit(1);
}
void ContextManager::printSymbolTable() {
    if (this->contextStack.size() > 0) {
        this->contextStack.top()->printSymbolTable();
    }
    this->globalContext->printSymbolTable();
}