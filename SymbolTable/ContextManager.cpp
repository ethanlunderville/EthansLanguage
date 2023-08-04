#include "ContextManager.h"
/* This code is kind of insane so be warned */
ContextManager::ContextManager(TypeManager* typeManager) {
    this->globalContext = new SymbolTable(nullptr);
    this->globalScopeLinkedList = nullptr;
    this->typeManager = typeManager;
}
ContextManager::~ContextManager() {
    while (this->contextStack.size() != 0) {
        SymbolTable* pointer = this->contextStack.top();
        contextPopRecurse(pointer);
        contextStack.pop();
    }
    contextPopRecurse(this->globalScopeLinkedList);
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
    if (sym == nullptr) {
        return;
    }
    if (sym->tableReference == nullptr) {
        delete sym;
        return;
    }
    SymbolTable* next = sym->tableReference;
    delete sym;
    contextPopRecurse(next);
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
    SymbolTable* pointer;
    if (contextStack.size() > 0) {
        pointer = this->contextStack.top();
    } else if (this->globalScopeLinkedList != nullptr) {
        pointer = this->globalScopeLinkedList;
    } else {
        this->globalScopeLinkedList = new SymbolTable(nullptr);
        return;
    }
    while (pointer->tableReference != nullptr) {
        pointer = pointer->tableReference;
    }
    pointer->tableReference = new SymbolTable(nullptr);
    return;
}

SymbolTable* ContextManager::popScope(bool isStruct){
    SymbolTable* pointer;
    if (this->contextStack.size() > 0) {
        SymbolTable* pointer = this->contextStack.top();
        if (pointer->tableReference == nullptr) {
            std::cerr << "unable to pop empty scope stack";
            exit(1);
        }
        while (pointer->tableReference->tableReference != nullptr) {
            pointer = pointer->tableReference;
        }
        if (isStruct) {
            SymbolTable* temp = pointer->tableReference;
            pointer->tableReference = nullptr;
            return temp;
        }
        delete pointer->tableReference;
        pointer->tableReference = nullptr;
        return nullptr;
    } else if (this->globalScopeLinkedList != nullptr) {
        pointer = this->globalScopeLinkedList;
        if (pointer == nullptr) {
            std::cerr << "unable to pop empty scope stack";
            exit(1);
        }
        if (pointer->tableReference == nullptr) {
            if (isStruct) {
                SymbolTable* temp = this->globalScopeLinkedList;
                this->globalScopeLinkedList = nullptr;
                return temp;
            }
            delete this->globalScopeLinkedList;
            this->globalScopeLinkedList = nullptr;
            return nullptr;
        }
        while (pointer->tableReference->tableReference != nullptr) {
            pointer = pointer->tableReference;
        }
        if (isStruct) {
            SymbolTable* temp = pointer->tableReference;
            pointer->tableReference = nullptr;
            return temp;
        }
        delete pointer->tableReference;
        pointer->tableReference = nullptr;
        return nullptr;
    } else {
        std::cerr << "Unable to pop empty scope stack" << std::endl;
        exit(1);
    }
}

void ContextManager::declareSymbol(int line, std::string identifier, Type* currentType) {
    SymbolTable* pointer;
    if (this->contextStack.size() > 0) {
        pointer = this->globalContext;
    } else if (this->globalScopeLinkedList != nullptr) {
        pointer = this->globalScopeLinkedList;
        if (this->globalContext->contains(identifier) != -1) {
            std::cerr << 
            "Incorrect declaration of already declared variable: " 
            << identifier  << " on line: " << line 
            << std::endl;
            exit(1);
        }
    } else {
        pointer = this->globalContext;
    }
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
void ContextManager::declareSymbol(int line, std::string identifier, std::string type) {
    Type* currentType = this->typeManager->getTypeHandler(type);
    SymbolTable* pointer;
    if (this->contextStack.size() > 0) {
        pointer = this->globalContext;
    } else if (this->globalScopeLinkedList != nullptr) {
        pointer = this->globalScopeLinkedList;
        if (this->globalContext->contains(identifier) != -1) {
            std::cerr << 
            "Incorrect declaration of already declared variable: " 
            << identifier  << " on line: " << line 
            << std::endl;
            exit(1);
        }
    } else {
        pointer = this->globalContext;
    }
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
    SymbolTable* pointer;
    if (this->contextStack.size() > 0) { 
        pointer = this->globalContext;
    } else if ( this->globalScopeLinkedList != nullptr) {
        pointer = this->globalScopeLinkedList;
        if (this->globalContext->contains(identifier) != -1 ) {
            if (pointer->variableTypeCheck(this->globalContext->getTypeOfSymbol(identifier), value)) {
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
    } else {
        pointer = this->globalContext;
    }
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
    SymbolTable* pointer;
    if (this->contextStack.size() > 0) {
        pointer = this->globalContext;
    } else if (this->globalScopeLinkedList != nullptr) {
        pointer = globalScopeLinkedList;
        if (this->globalContext->contains(identifier) != -1) {
            return this->globalContext->getValueStoredInSymbol(identifier);
        }
    } else {
        pointer = this->globalContext;
    }
    while (pointer != nullptr) {
        if (pointer->contains(identifier) != -1) {
            return pointer->getValueStoredInSymbol(identifier);
        }
        pointer = pointer->tableReference;
    }
    std::cerr << "Unrecognized identifier: " << identifier <<std::endl;
    exit(1);
}


std::any* ContextManager::getReferenceOfValueStoredInSymbol(std::string identifier) {
    SymbolTable* pointer;
    if (this->contextStack.size() > 0) {
        pointer = this->globalContext;
    } else if (this->globalScopeLinkedList != nullptr) {
        pointer = globalScopeLinkedList;
        if (this->globalContext->contains(identifier) != -1) {
            return this->globalContext->getReferenceOfValueStoredInSymbol(identifier);
        }
    } else {
        pointer = this->globalContext;
    }
    while (pointer != nullptr) {
        if (pointer->contains(identifier) != -1) {
            return pointer->getReferenceOfValueStoredInSymbol(identifier);
        }
        pointer = pointer->tableReference;
    }
    std::cerr << "Unrecognized identifier: " << identifier <<std::endl;
    exit(1);
}

std::any ContextManager::getValueStoredInSymbol(std::string identifier, int subscript) {
    SymbolTable* pointer;
    if (this->contextStack.size() > 0) {
        pointer = this->globalContext;
    } else if (this->globalScopeLinkedList != nullptr) {
        pointer = globalScopeLinkedList;
        if (this->globalContext->contains(identifier) != -1) {
            return this->globalContext->getValueStoredInSymbol(identifier, subscript);
        }
    } else {
        pointer = this->globalContext;
    }
    while (pointer != nullptr) {
        if (pointer->contains(identifier) != -1) {
            return pointer->getValueStoredInSymbol(identifier, subscript);
        }
        pointer = pointer->tableReference;
    }
    std::cerr << "Unrecognized identifier: " << identifier <<std::endl;
    exit(1);
} 

std::any* ContextManager::getReferenceOfValueStoredInSymbol(std::string identifier, int subscript) {
    SymbolTable* pointer;
    if (this->contextStack.size() > 0) {
        pointer = this->globalContext;
    } else if (this->globalScopeLinkedList != nullptr) {
        pointer = globalScopeLinkedList;
        if (this->globalContext->contains(identifier) != -1) {
            return this->globalContext->getReferenceOfValueStoredInSymbol(identifier, subscript);
        }
    } else {
        pointer = this->globalContext;
    }
    while (pointer != nullptr) {
        if (pointer->contains(identifier) != -1) {
            return pointer->getReferenceOfValueStoredInSymbol(identifier, subscript);
        }
        pointer = pointer->tableReference;
    }
    std::cerr << "Unrecognized identifier: " << identifier <<std::endl;
    exit(1);
}


Type* ContextManager::getTypeOfSymbol(std::string identifier) {
    SymbolTable* pointer;
    if (this->contextStack.size() > 0) {
        pointer = this->globalContext;
    } else if (this->globalScopeLinkedList != nullptr) {
        pointer = globalScopeLinkedList;
        if (this->globalContext->contains(identifier) != -1) {
            return this->globalContext->getTypeOfSymbol(identifier);
        }
    } else {
        pointer = this->globalContext;
    }
    while (pointer != nullptr) {
        if (pointer->contains(identifier) != -1) {
            return pointer->getTypeOfSymbol(identifier);
        }
        pointer = pointer->tableReference;
    }
    std::cerr << "Unrecognized identifier: " << identifier <<std::endl;
    exit(1);
}
void ContextManager::setCurrentFunctionType(Type* t) {
    if (this->contextStack.size() == 0) {
        std::cerr << "COMPILER ERROR::There are no contexts so setting a type is not possible" << std::endl;
        exit(1);
    }
    this->contextStack.top()->setCurrentFunctionType(t);
}
Type* ContextManager::getCurrentFunctionType() {
    if (this->contextStack.size() == 0) {
        std::cerr << "COMPILER ERROR::There are no contexts so getting a type is not possible" << std::endl;
        exit(1);
    }
    return this->contextStack.top()->getCurrentFunctionType();
}

bool ContextManager::isFunctionReturned() {
    if (this->contextStack.size() > 0) {
        if (this->contextStack.top()->returned) {
            return true;
        }
        return false;
    }
    return false;
}
void ContextManager::setFunctionIsReturned(bool isReturned) {
    if (this->contextStack.size() > 0) {
        this->contextStack.top()->returned = isReturned;
    }
}

void ContextManager::printSymbolTable() {
    SymbolTable* pointer;
    std::cout << "---- ***Table*** ---" << std::endl;
    if (this->contextStack.size() > 0) {
        pointer = this->globalContext;
    } else {
        this->globalContext->printSymbolTable();
        pointer = this->globalScopeLinkedList;
    }
    while (pointer != nullptr) {
        pointer->printSymbolTable();
        pointer = pointer->tableReference;
    }
    return;
}