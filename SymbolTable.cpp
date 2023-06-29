#include "SymbolTable.h"

SymbolTable::SymbolTable(SymbolTable* tableReference) : globalTable(tableReference) {
    pushScope();
}

void SymbolTable::pushScope() {
    scopeStack.push(this->getCurrentSize());
}

void SymbolTable::popScope(){
    if (scopeStack.size() == 0) {
        std::cerr << "EMPTY SCOPE" << std::endl;
    }
    int popCounter = this->getCurrentSize() - scopeStack.top();
    scopeStack.pop();
    for (int i = 0 ; i < popCounter ; i++) {
        stringToSymbolMap.erase(intToStringVector[this->getCurrentSize() - 1]);
        intToStringVector.erase(intToStringVector.begin() + (this->getCurrentSize() - 1));
    }
}

void SymbolTable::declareSymbol(int line, std::string identifier , std::any value, std::string type) {
    if (
        value.type() == typeid(std::string) && type.compare("string") == 0 ||
        value.type() == typeid(int) && type.compare("int") == 0
       ) {
            this->intToStringVector.push_back(identifier);
            this->stringToSymbolMap[this->intToStringVector[this->getCurrentSize() - 1]] = {line, value, type};
            return;
    }
    std::cerr << "Incorrect type assignment for variable: " << identifier << std::endl;
    exit(1);
}

void SymbolTable::reassignSymbol(std::string identifier , std::any value) {  
    if (
        ((value.type() == typeid(std::string) && this->stringToSymbolMap[identifier].type.compare("string") == 0)||
        (value.type() == typeid(int) && this->stringToSymbolMap[identifier].type.compare("int") == 0))
       ) {
        this->stringToSymbolMap[identifier].value = value;
    }
    std::cerr << "Incorrect type assignment for variable: " << identifier << std::endl;
    exit(1);
}

std::string SymbolTable::getTypeOfSymbol(std::string identifier) {
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

void SymbolTable::printSymbolTable() {
    std::cout << "---- ***Table*** ---" << std::endl;
    int line = 1;
    for (int i = (intToStringVector.size()-1) ; i > -1 ; i--) {
        if (stringToSymbolMap[intToStringVector[i]].type.compare("int") == 0) {
            std::cout << "Identifier: " << intToStringVector[i] 
            << "\tValue: " << std::any_cast<int>(stringToSymbolMap[intToStringVector[i]].value) 
            << std::endl;
        } else if (stringToSymbolMap[intToStringVector[i]].type.compare("string") == 0) {
            std::cout << "Identifier: " << intToStringVector[i] 
            << "\tValue: " << std::any_cast<std::string>(stringToSymbolMap[intToStringVector[i]].value) 
            << std::endl;
        }
        line++;
    }
    if (globalTable != nullptr) {
        globalTable->printSymbolTable();
    }
}

 
