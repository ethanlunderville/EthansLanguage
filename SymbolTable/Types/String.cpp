#include "Types.h"
#include "SymbolTable/ContextManager.h"

String::String() {}


std::any String::getNullValue() {
    return std::string("");
}

AST* String::getNewTreenode(std::string value) {
    return new StringTree(value);
}

bool String::checkType(std::any value) {
    if (value.type() == typeid(std::string)) {
        return true;
    }
    return false;
}

void String::printType() {
    std::cout << "String";
}

void String::printSymbol(std::string identifier, std::any symbol) {
    std::cout << "Identifier: " << identifier 
    << "\tValue: " << std::any_cast<std::string>(symbol) 
    << std::endl;
}

void String::printArrayOfType(std::any vector) {
    std::vector<std::any> arr = std::any_cast<std::vector<std::any>>(vector);
    int size = arr.size();
    for (int i = 0 ; i < size ; i++) {
        std::cout << std::any_cast<std::string>(arr[i]);
        if (i != size - 1) {
            std::cout << ", ";
        } 
    }
}