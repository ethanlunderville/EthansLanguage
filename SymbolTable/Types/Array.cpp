#include "Types.h"
#include "SymbolTable/ContextManager.h"

Array::Array(Type* arrayType) {
    this->type = arrayType;
}

Array::~Array() {}

std::any Array::getBaseArray() {
    return 0;
}

bool Array::checkExpression(AST* node, int line, ContextManager* contextManager) {
    return this->type->checkExpression(node, line, contextManager);
}

void Array::checkAssignment(Assignable* assign) {
    this->type->checkAssignment(assign);
}

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
    return this->type->getBaseArray();
}