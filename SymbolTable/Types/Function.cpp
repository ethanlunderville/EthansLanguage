#include "Types.h"
#include "SymbolTable/ContextManager.h"

Function::Function(Type* returnType) {
    this->type = returnType;
}

Function::~Function() {}

std::any Function::getBaseArray() {
    return 0;
}

bool Function::checkExpression(AST* node, int line, ContextManager* contextManager) {
    return false;
}

void Function::checkAssignment(Assignable* assign) {
    this->type->checkAssignment(assign);
}

bool Function::checkType(std::any value) {
    return true;
    //assign->checkType(this);
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
    return this->type->getBaseArray();
};