#include "Types.h"
#include "SymbolTable/ContextManager.h"

Array::Array(Type* arrayType) {
    this->type = arrayType;
}

Array::~Array() {}

bool Array::checkType(std::any value) {
    return false;
}
void Array::printSymbol(std::string identifier, std::any symbol) {
    std::cout << "Identifier: " << identifier
    << "\tValue: " << std::any_cast<double>(symbol) 
    << std::endl;
}

std::any Array::getNullValue() {
    return 1;
}