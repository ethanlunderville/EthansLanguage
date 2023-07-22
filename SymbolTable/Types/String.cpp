#include "Types.h"
#include "SymbolTable/ContextManager.h"

String::String() {}

std::any String::getNullValue() {
    return std::string("");
}

AST* String::getNewTreenode(std::string value) {
    return new StringTree(value);
}

AST* String::getExpressionNode() {
    return nullptr;
}

bool String::checkType(std::any value) {
    return value.type() == typeid(std::string);
}

void String::printSymbol(std::string identifier, std::any symbol) {
    std::cout << "Identifier: " << identifier 
    << "\tValue: " << std::any_cast<std::string>(symbol) 
    << std::endl;
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


bool String::checkTree(AST* node, int line, ContextManager* contextManager) {
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
        if (!checkTree(stringExpressionNode, line , contextManager)) {
            return false;
        }
        if (typeid(*node) == typeid(StringTree)) {
            stringNumCounter++;
        }
    }
    return true;
}

bool String::changeExpressionToDeclaredTypeIfItIsLegal(AST* node, int line, ContextManager* contextManager) {
    if (!checkTree(node, line,contextManager)) {
        std::cerr << "Error: Incorrect string expression on line " << line << std::endl;
        std::cerr << "Expression must contain at least one string or string identifier " << line << std::endl;
        return false;
    }
    stringNumCounter = 0;
    return true;
}