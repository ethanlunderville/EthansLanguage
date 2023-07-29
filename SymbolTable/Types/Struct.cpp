#include "Types.h"
#include "SymbolTable/ContextManager.h"
class SymbolTable;

Struct::Struct(SymbolTable* baseStruct) {
    this->baseStruct = baseStruct;
}

std::any Struct::getBaseArray() {
    std::vector<SymbolTable*> structArray;
    return structArray;
}

void Struct::checkAssignment(Assignable* assign) {
    assign->checkType(this);
}

bool Struct::checkType(std::any value) {
    return true;
    //assign->checkType(this);
}

bool Struct::checkExpression(AST* node, int line, ContextManager* contextManager) {
    return true;
}

std::any Struct::getNullValue() {
    return new SymbolTable(*(this->baseStruct));
}

void Struct::printType() {
    std::cout << "Struct";
}

void Struct::printSymbol(std::string identifier, std::any symbol) {
    SymbolTable* structHold = std::any_cast<SymbolTable*>(symbol);
    std::cout << "struct: " << identifier << "-> {" << std::endl;
    for (int i = 0 ;i < structHold->intToStringVector.size();i++) {
        SymbolInfo sInfo = structHold->stringToSymbolMap[structHold->intToStringVector[i]];
        std::cout << "   " << structHold->intToStringVector[i] << " : ";
        sInfo.type->printSymbol(structHold->intToStringVector[i], sInfo.value);
    }
    std::cout << "}" << std::endl;
}
void Struct::printArrayOfType(std::any vector) {

}
