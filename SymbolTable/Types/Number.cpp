#include "Types.h"
#include "SymbolTable/ContextManager.h"


Number::Number() {}

std::any Number::getBaseArray() {
    std::vector<double> doubleVector;
    return doubleVector;
}

std::any Number::getNullValue() {
    return std::any(0.00);
}

AST* Number::getNewTreenode(std::string value) {
    return new NumberTree(value);
}

void Number::checkAssignment(Assignable* assign) {
    assign->checkType(this);
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
    std::cout << "[ Number ] -> ";
    std::vector<double> arr = std::any_cast<std::vector<double>>(vector);
    int size = arr.size();
    std::cout << "[ ";
    for (int i = 0 ; i < size ; i++) {
        std::cout << arr[i];
        if (i != size - 1) {
            std::cout << ", ";
        } 
    }
    std::cout << " ]\n";
}

bool Number::checkExpression(AST* node, int line, ContextManager* contextManager) {
    for (AST* child : node->getChildren()) {
        if (typeid(*child) == typeid(StringTree)) {
            std::cerr << "Invlalid string found on line " << line <<  std::endl;
            std::cerr << "Integer expressions can not evaluate include string" <<  std::endl;
            return false;
        } else if (typeid(*child) == typeid(IdentifierTree)) {
            IdentifierTree* ident = dynamic_cast<IdentifierTree*>(child);
            if (typeid(*(contextManager->getTypeOfSymbol(ident->getIdentifier()))) == typeid(String)) {
                std::cerr << "Invlalid string identifier found on line " << line <<  std::endl;
                std::cerr << "Integer expressions can not evaluate include string" <<  std::endl;
                return false;
            } 
        }
        if (!checkExpression(child, line, contextManager)) {
            return false;
        }
    }
    return true;
}