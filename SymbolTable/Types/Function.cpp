#include "Types.h"
#include "SymbolTable/ContextManager.h"

Function::Function(Type* returnType) : type(returnType) {}

Function::~Function() {}

bool Function::checkType(std::any value) {
    return false;
}
Type* Function::getFunctionType() {
    return this->type;
}

void Function::printArrayOfType(std::any vector) {

}


void Function::printSymbol(std::string identifier, std::any symbol) {
    std::cout << "function: " << identifier << " -> ";
    this->type->printType();
    std::cout << "" << std::endl;
}

void Function::printType() {
    std::cout << "Function";
}

std::any Function::getNullValue() {
    return this->type->getNullValue();
};