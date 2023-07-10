#include "Types.h"

Number::Number() {}

std::any Number::getNullValue() {
    return std::any(0.00);
}

bool Number::checkType(std::any value) {
    return value.type() == typeid(double);
}

void Number::printSymbol(std::string identifier, std::any symbol) {
    std::cout << "Identifier: " << identifier
    << "\tValue: " << std::any_cast<double>(symbol) 
    << std::endl;
}

bool Number::checkIfExpressionOfThisTypeIsValid(AST* node, int line) {
    return true;
}