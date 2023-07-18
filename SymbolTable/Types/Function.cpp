#include "Types.h"
#include "SymbolTable/ContextManager.h"

Function::Function(Type* returnType) {
    this->type = returnType;
}

Function::~Function() {}

bool Function::checkType(std::any value) {
    return false;
}

void Function::printSymbol(std::string identifier, std::any symbol) {
    std::cout << "Identifier: " << identifier
    << "\tValue: " << std::any_cast<double>(symbol) 
    << std::endl;
}

std::any Function::getNullValue() {
    return 1;
};