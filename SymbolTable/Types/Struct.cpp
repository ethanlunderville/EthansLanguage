#include "Types.h"
#include "SymbolTable/ContextManager.h"
class SymbolTable;

Struct::Struct(SymbolTable* baseStruct, const std::string& identifier) {
    this->baseStruct = baseStruct;
    this->structIdentifier = identifier;
    this->baseStruct->setTypeName(this->structIdentifier);
}

Struct::Struct(const std::string& identifier) {
    this->structIdentifier = identifier;
}

std::any Struct::getBaseArray() {
    std::vector<SymbolTable*> structArray;
    return structArray;
}

void Struct::checkAssignment(Assignable* assign) {
    assign->checkType(this);
}

bool Struct::checkType(std::any value) {
    if (value.type() == typeid(SymbolTable*)) {
        if (this->structIdentifier.compare(std::any_cast<SymbolTable*>(value)->getTypeName()) == 0) {
            return true;
        }
    }
    return false;
}

bool Struct::checkExpression(AST* node, int line, ContextManager* contextManager) {
    return true;
}

std::any Struct::getNullValue() {
    return nullptr;
}

SymbolTable* Struct::getBaseStructPointer() {
    return this->baseStruct;
}

void Struct::setBaseStructPointer(SymbolTable* base) {
    this->baseStruct = base;
    this->baseStruct->setTypeName(this->structIdentifier);
}

SymbolTable* Struct::getDuplicateBase() {
    SymbolTable* sTable = new SymbolTable(*(this->baseStruct));
    //SymbolTable* sTable = new SymbolTable(this->baseStruct);
    sTable->setTypeName(this->structIdentifier);
    return sTable;
}

void Struct::printType() {
    std::cout << "Struct";
}

void Struct::printSymbol(std::string identifier, std::any symbol) {
    std::cout << "\n";
    if (symbol.type() == typeid(nullptr)) {
        std::cout << identifier << " -> NULL" << std::endl;
        return;
    }
    SymbolTable* structHold = std::any_cast<SymbolTable*>(symbol);
    std::cout << structHold->getTypeName()  << ": " << identifier << " -> " << structHold << " {" << std::endl;
    for (int i = 0 ;i < structHold->intToStringVector.size();i++) {
        SymbolInfo sInfo = structHold->stringToSymbolMap[structHold->intToStringVector[i]];
        std::cout << "   " << structHold->intToStringVector[i] << " : ";
        sInfo.type->printSymbol(structHold->intToStringVector[i], sInfo.value);
    }
    std::cout << "}" << std::endl;
}

void Struct::printSymbol(std::any symbol) {
    std::cout << "\n";
    if (symbol.type() == typeid(nullptr) || !symbol.has_value()) {
        std::cout << "? -> NULL" << std::endl;
        return;
    }
    SymbolTable* structHold = std::any_cast<SymbolTable*>(symbol);
    std::cout << structHold->getTypeName()  << ": ? -> " << structHold << " {" << std::endl;
    for (int i = 0 ;i < structHold->intToStringVector.size();i++) {
        SymbolInfo sInfo = structHold->stringToSymbolMap[structHold->intToStringVector[i]];
        std::cout << "   " << structHold->intToStringVector[i] << " : ";
        sInfo.type->printSymbol(structHold->intToStringVector[i], sInfo.value);
    }
    std::cout << "}" << std::endl;
}

void Struct::printArrayOfType(std::any vector) {
    std::vector<std::any> arr = std::any_cast<std::vector<std::any>>(vector);
    for ( int i = 0 ; i < arr.size() ; i++ ) {
        std::cout << "   ";
        this->printSymbol(std::any(arr[i]));
    }
}
