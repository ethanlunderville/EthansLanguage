#include "Types.h"
#include "SymbolTable/ContextManager.h"


Number::Number() {}

std::any Number::getNullValue() {
    return std::any(0.00);
}

AST* Number::getNewTreenode(std::string value) {
    return new NumberTree(value);
}

bool Number::checkType(std::any value) {
    if (value.type() == typeid(double)) {
        return true;
    }
    return false;
}

void Number::printSymbol(std::string identifier, std::any symbol) {
    std::cout << "Identifier: " << identifier
    << "\tValue: " << std::any_cast<double>(symbol) 
    << std::endl;
}

void Number::printType() {
    std::cout << "Number";
}

void Number::printArrayOfType(std::any vector) {
    std::vector<std::any> arr = std::any_cast<std::vector<std::any>>(vector);
    int size = arr.size();
    for (int i = 0 ; i < size ; i++) {
        std::cout << std::any_cast<double>(arr[i]);
        if (i != size - 1) {
            std::cout << ", ";
        } 
    }
}
