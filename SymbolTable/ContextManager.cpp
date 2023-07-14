#include "ContextManager.h"

ContextManager::ContextManager(TypeManager* typeManager) {
    this->globalContext = new SymbolTable(nullptr);
    this->typeManager = typeManager;
}
ContextManager::~ContextManager() {
    while (this->contextStack.size() != 0) {
        SymbolTable* pointer = this->contextStack.top();
        contextPopRecurse(pointer);
    }
    delete this->globalContext;
    this->globalContext = nullptr; 
    delete this->typeManager;
    this->typeManager = nullptr;
}
void ContextManager::pushContext() {
    SymbolTable* sTable = new SymbolTable(nullptr);
    this->globalContext->tableReference = sTable;
    this->contextStack.push(sTable);
}

void ContextManager::contextPopRecurse(SymbolTable* sym) {
    if (sym->tableReference == nullptr) {
        delete this;
        return;
    }
    contextPopRecurse(sym);
    delete this;
    return;
}

void ContextManager::popContext() {
    if (this->contextStack.size() > 0) {
        SymbolTable* pointer = this->contextStack.top();
        contextStack.pop();
        contextPopRecurse(pointer);
        if (contextStack.size() != 0){
            this->globalContext->tableReference = contextStack.top();
            return;
        }
        this->globalContext->tableReference = nullptr;
        return;
    }
    std::cerr << "Unable to pop empty context stack" << std::endl;
    exit(1);
}
void ContextManager::pushScope() {
    SymbolTable* pointer = this->globalContext;
    while (pointer != nullptr) {
        pointer = pointer->tableReference;
    }
    pointer = new SymbolTable(nullptr);
}

void ContextManager::popScope(){
    SymbolTable* pointer = this->globalContext;
    while (pointer->tableReference != nullptr) {
        pointer = pointer->tableReference;
    }
    if (pointer != this->globalContext) {
        delete pointer;
        pointer = nullptr;
    }
    std::cerr << "Unable to pop empty scope stack" << std::endl;
    exit(1);
}
void ContextManager::declareSymbol(int line, std::string identifier, std::string type) {
    Type* currentType = this->typeManager->getTypeHandler(type);
    SymbolTable* pointer = this->globalContext;
    while (pointer != nullptr) {
        if (pointer->contains(identifier) != -1) {
            std::cerr << 
            "Incorrect declaration of already declared variable: " 
            << identifier  << " on line: " << line 
            << std::endl;
            exit(1);
        }
        if (pointer->tableReference == nullptr && pointer->contains(identifier) == -1) {
            pointer->declareSymbol(line, identifier, currentType);
            return;
        }
        pointer = pointer->tableReference;
    } 
    return;
}   
void ContextManager::reassignSymbol(std::string identifier , std::any value, int line) { 
    SymbolTable* pointer = this->globalContext;
    while (pointer != nullptr) {
        if (pointer->contains(identifier) != -1 ) {
            if (pointer->variableTypeCheck(pointer->getTypeOfSymbol(identifier), value)) {
                pointer->reassignSymbol(identifier, value);
                return;
            } else {
                std::cerr <<
                "Incorrect reassignment of undeclared variable: " 
                << identifier  << " on line: " << line
                << " due to incorrect type."   
                << std::endl;
                exit(1);
            }
        }
        pointer = pointer->tableReference;
    }
    std::cerr <<
    "Incorrect reassignment of undeclared variable: " 
    << identifier  << " on line: " << line 
    << std::endl;
    exit(1);
}   
std::any ContextManager::getValueStoredInSymbol(std::string identifier) {
SymbolTable* pointer = this->globalContext;
    while (pointer != nullptr) {
        if (pointer->contains(identifier) != -1) {
            return pointer->getValueStoredInSymbol(identifier);
        }
        pointer = pointer->tableReference;
    }
    std::cerr << "Unrecognized identifier: " << identifier <<std::endl;
    exit(1);
}
Type* ContextManager::getTypeOfSymbol(std::string identifier) {
    SymbolTable* pointer = this->globalContext;
    while (pointer != nullptr) {
        if (pointer->contains(identifier) != -1) {
            return pointer->getTypeOfSymbol(identifier);
        }
        pointer = pointer->tableReference;
    }
    std::cerr << "Unrecognized identifier: " << identifier <<std::endl;
    exit(1);
}
void ContextManager::printSymbolTable() {
    SymbolTable* pointer = this->globalContext;
    while (pointer != nullptr) {
        pointer->printSymbolTable();
        pointer = pointer->tableReference;
    }
}