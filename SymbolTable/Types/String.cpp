#include "Types.h"
#include "SymbolTable/ContextManager.h"

String::String() {}

std::any String::getBaseArray() {
    std::vector<std::string> stringVector;
    return stringVector;
}

std::any String::getNullValue() {
    return std::string("");
}

AST* String::getNewTreenode(std::string value) {
    return new StringTree(value);
}

void String::checkAssignment(Assignable* assign) {
    assign->checkType(this);
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

static int stringNumCounter = 0;

bool String::numberCheck(AST* node, int line, ContextManager* contextManager) {
    for (AST* child : node->getChildren()) {
        if (typeid(*child) == typeid(StringTree)) {
            std::cerr << "Invlalid string found on line " << line <<  std::endl;
            std::cerr << "Cannot create subexpressions with strings" <<  std::endl;
            exit(1);
        } else if (typeid(*child) == typeid(IdentifierTree)) {
            IdentifierTree* ident = dynamic_cast<IdentifierTree*>(child);
            if (typeid(*(contextManager->getTypeOfSymbol(ident->getIdentifier()))) == typeid(String)) {
                std::cerr << "Invlalid string found on line " << line <<  std::endl;
                std::cerr << "Cannot create subexpressions with strings" <<  std::endl;
                exit(1);
            } 
        }
        if (!numberCheck(child, line, contextManager)) {
            return false;
        }
    }
    return true;
}


bool String::checkExpression(AST* node, int line, ContextManager* contextManager) {
    for (AST* stringExpressionNode : node->getChildren()) {
        bool nodeTypeIsExpressionTree = typeid(*stringExpressionNode) == typeid(ExpressionTree);
        if (typeid(*stringExpressionNode) != typeid(NumberTree) &&
            typeid(*stringExpressionNode) != typeid(AddTree) &&
            typeid(*stringExpressionNode) != typeid(StringTree) && 
            !nodeTypeIsExpressionTree) {
            return false;
        }
        if (nodeTypeIsExpressionTree) {
            if (!this->numberCheck(stringExpressionNode, line, contextManager)) {
                return false;
            }
            continue;
        }
        if (!checkExpression(stringExpressionNode, line , contextManager)) {
            return false;
        }
        if (typeid(*node) == typeid(StringTree)) {
            stringNumCounter++;
        }
    }
    return true;
}
