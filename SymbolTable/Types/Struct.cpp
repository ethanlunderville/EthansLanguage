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

std::any Struct::getNullValue() {return nullptr;}
void Struct::printSymbol(std::string identifier, std::any symbol) {}
void Struct::printArrayOfType(std::any vector) {

}
