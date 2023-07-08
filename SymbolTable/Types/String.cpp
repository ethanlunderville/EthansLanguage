#include "Types.h"

String::String() {}

std::any String::getNullValue() {
    return std::string("");
}

bool String::checkType(std::any value) {
    return value.type() == typeid(std::string);
}

void String::printSymbol(std::string identifier, std::any symbol) {
    std::cout << "Identifier: " << identifier 
    << "\tValue: " << std::any_cast<std::string>(symbol) 
    << std::endl;
}

void String::handleSymbol(AST* node) {

}