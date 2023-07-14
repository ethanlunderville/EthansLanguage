#include "SymbolTable.h"

SymbolTable::SymbolTable(SymbolTable* tableReference) : tableReference(tableReference) {
    pushScope();
}

void SymbolTable::pushScope() {
    scopeStack.push(this->getCurrentSize());
}

void SymbolTable::popScope(){
    if (scopeStack.size() == 0) {
        std::cerr << "Cannot pop an empty scope" << std::endl;
    }
    int popCounter = this->getCurrentSize() - scopeStack.top();
    scopeStack.pop();
    for (int i = 0 ; i < popCounter ; i++) {
        stringToSymbolMap.erase(intToStringVector[this->getCurrentSize() - 1]);
        intToStringVector.erase(intToStringVector.begin() + (this->getCurrentSize() - 1));
    }
}

void SymbolTable::declareSymbol(int line, std::string identifier, Type* typeHandler) {
    this->intToStringVector.push_back(identifier);
    this->stringToSymbolMap[this->intToStringVector[this->getCurrentSize() - 1]] = {line, typeHandler->getNullValue(), typeHandler};
    return;
}

void SymbolTable::reassignSymbol(std::string identifier , std::any value) {  
    this->stringToSymbolMap[identifier].value = value;
}

Type* SymbolTable::getTypeOfSymbol(std::string identifier) {
    return this->stringToSymbolMap[identifier].type;
}

std::any SymbolTable::getValueStoredInSymbol(std::string identifier) {
    return this->stringToSymbolMap[identifier].value;
}

int SymbolTable::getCurrentSize() {
    return this->intToStringVector.size();
}

int SymbolTable::contains(std::string identifier) {
    for (int i = 0; i < this->getCurrentSize(); i++) {
        if (identifier.compare(intToStringVector[i]) == 0) {
            return i;
        }
    }
    return -1;
}

bool SymbolTable::variableTypeCheck(Type* typeHandler , std::any value) {
    if (typeHandler->checkType(value)) {
        return true;
    }
    return false;
}

void SymbolTable::printSymbolTable() {
    std::cout << "---- ***Table*** ---" << std::endl;
    for (int i = (intToStringVector.size()-1) ; i > -1 ; i--) {
        stringToSymbolMap[intToStringVector[i]].type
        ->printSymbol(intToStringVector[i], stringToSymbolMap[intToStringVector[i]].value);
    }
}

 
