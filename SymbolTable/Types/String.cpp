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

bool String::handleSymbol(AST* node, int line) {
    for (AST* stringExpressionNode : node->getChildren()) {
        bool nodeTypeIsExpressionTree = typeid(stringExpressionNode) == typeid(ExpressionTree);
        if (
            typeid(stringExpressionNode) != typeid(AddTree) &&
            typeid(stringExpressionNode) != typeid(StringTree) && !nodeTypeIsExpressionTree
            ) {
            std::cerr << "\nError: Incorrect expression on line " << line << std::endl;
            std::cerr << "string assignment expressions may only contain" << std::endl;
            std::cerr << "strings or string identifiers that can optionally" << std::endl;
            std::cerr << "be seperated by a concatenation operator: +\n" << std::endl;
            return false;
        }
        return this->handleSymbol(stringExpressionNode, line);
    }
    return true;
}