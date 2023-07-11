#include "Types.h"

String::String() {}

std::any String::getNullValue() {
    return std::string("");
}

AST* String::getNewTreenode(std::string value) {
    return new StringTree(value);
}

AST* String::getExpressionNode() {
    return new StringExpressionTree();
}

bool String::checkType(std::any value) {
    return value.type() == typeid(std::string);
}

void String::printSymbol(std::string identifier, std::any symbol) {
    std::cout << "Identifier: " << identifier 
    << "\tValue: " << std::any_cast<std::string>(symbol) 
    << std::endl;
}

bool String::checkIfExpressionOfThisTypeIsValid(AST* node, int line) {
    for (AST* stringExpressionNode : node->getChildren()) {
        bool nodeTypeIsExpressionTree = typeid(*stringExpressionNode) == typeid(ExpressionTree);
        if (typeid(*stringExpressionNode) != typeid(NumberTree) &&
            typeid(*stringExpressionNode) != typeid(AddTree) &&
            typeid(*stringExpressionNode) != typeid(StringTree) && !nodeTypeIsExpressionTree) {
            std::cerr << "\nError: Incorrect expression on line " << line << std::endl;
            std::cerr << "string assignment expressions may only contain" << std::endl;
            std::cerr << "strings or string identifiers that can optionally" << std::endl;
            std::cerr << "be separated by a concatenation operator: +\n" << std::endl;
            return false;
        }
        if (nodeTypeIsExpressionTree) {
            continue;
        }
        if (!this->checkIfExpressionOfThisTypeIsValid(stringExpressionNode, line)) {
            return false;
        }
    }
    return true;
}
