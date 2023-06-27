#include "Table.h"

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
    int potentialLineNum = this->contains(identifier);
    int potentialGlobalLineNum = -1;
    if (this->globalTable != nullptr) {
        potentialGlobalLineNum = this->globalTable->contains(identifier);
    }
    if (potentialLineNum != -1 || potentialGlobalLineNum != -1) {
        std::cerr << "Multiple declarations of " << identifier << " on line " << 
        ((potentialLineNum == -1) ? potentialGlobalLineNum : potentialLineNum) << std::endl;
        exit(1);
    } else {
        this->intToStringVector.push_back(identifier);
        this->stringToSymbolMap[this->intToStringVector[this->getCurrentSize() - 1]] = {line, value, type};
    }
}

void SymbolTable::reassignSymbol(std::string identifier , std::any value) {
    if(stringToSymbolMap.count(identifier) > 0) { 
        this->stringToSymbolMap[identifier].value = value;
    } else {
        if (globalTable != nullptr) {
            globalTable->reassignSymbol(identifier, value);
            return;
        }
        std::cerr << "Incorrect assignment of undeclared variable: " << identifier << std::endl;
        exit(1);
    }
}

std::string SymbolTable::getTypeOfSymbol(std::string identifier) {
    return this->stringToSymbolMap[identifier].type;
}

auto SymbolTable::getValueStoredInSymbol(std::string identifier) {
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
        std::cout << line << ".\tIdentifier: " << intToStringVector[i] 
        << "\tValue: " << std::any_cast<int>(stringToSymbolMap[intToStringVector[i]].value) 
        << std::endl;
        line++;
    }
    if (globalTable != nullptr) {
        globalTable->printSymbolTable(line);
    }
}

void SymbolTable::printSymbolTable(int continuationIndex) {
    std::cout << "---- ***Table*** ---" << std::endl;
    int line = continuationIndex;
    for (int i = (intToStringVector.size()-1) ; i > -1 ; i--) {
        std::cout << line << ".\tIdentifier: " << intToStringVector[i] 
        << "\tValue: " << std::any_cast<int>(stringToSymbolMap[intToStringVector[i]].value) 
        << std::endl;
        line++;
    }
    if (globalTable != nullptr) {
        globalTable->printSymbolTable();
    }
}

 
