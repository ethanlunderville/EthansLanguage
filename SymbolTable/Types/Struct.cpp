#include "Types.h"
#include "SymbolTable/ContextManager.h"
class SymbolTable;

Struct::Struct(SymbolTable* baseStruct, const std::string& identifier) {
    this->baseStruct = baseStruct;
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
    if (this->structIdentifier.compare(std::any_cast<SymbolTable*>(value)->getStructName()) == 0) {
        return true;
    }
    return false;
}

bool Struct::checkExpression(AST* node, int line, ContextManager* contextManager) {
    return true;
}

std::any Struct::getNullValue() {
    return nullptr;
}

SymbolTable* Struct::getDuplicateBase() {
    SymbolTable* sTable = new SymbolTable(*(this->baseStruct));
    sTable->setStructName(this->structIdentifier);
    return sTable;
}

void Struct::printType() {
    std::cout << "Struct";
}

void Struct::printSymbol(std::string identifier, std::any symbol) {
    if (symbol.type() == typeid(nullptr)) {
        std::cout << "struct: " << identifier << " -> NULL" << std::endl;
        return;
    }
    SymbolTable* structHold = std::any_cast<SymbolTable*>(symbol);
    std::cout << "struct: " << identifier << " -> " << structHold << " {" << std::endl;
    for (int i = 0 ;i < structHold->intToStringVector.size();i++) {
        SymbolInfo sInfo = structHold->stringToSymbolMap[structHold->intToStringVector[i]];
        std::cout << "   " << structHold->intToStringVector[i] << " : ";
        sInfo.type->printSymbol(structHold->intToStringVector[i], sInfo.value);
    }
    std::cout << "}" << std::endl;
}
void Struct::printArrayOfType(std::any vector) {

}
