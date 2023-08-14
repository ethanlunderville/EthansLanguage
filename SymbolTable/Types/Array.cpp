#include "Types.h"
#include "SymbolTable/ContextManager.h"

Array::Array(Type* arrayType) : type(arrayType) {}

Array::~Array() {}

bool Array::checkType(std::any value) {
    return true;
}

Type* Array::getArrayType() {
    return this->type;
}

void Array::printSymbol(std::string identifier, std::any symbol) {
    std::cout << "Identifier: " << identifier << " = [";
    this->type->printArrayOfType(symbol);
    std::cout << "]" << std::endl;
}

void Array::printArrayOfType(std::any vector) {

}

void Array::printType() {
    std::cout << "Array";
}

std::any Array::getNullValue() {
    std::vector<std::any> anyVec;
    return anyVec;
}