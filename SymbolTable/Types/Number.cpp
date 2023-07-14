#include "Types.h"
#include "SymbolTable/ContextManager.h"


Number::Number() {}

std::any Number::getNullValue() {
    return std::any(0.00);
}

AST* Number::getNewTreenode(std::string value) {
    return new NumberTree(value);
}

AST* Number::getExpressionNode() {
    return new ExpressionTree();
}

bool Number::checkType(std::any value) {
    return value.type() == typeid(double);
}

void Number::printSymbol(std::string identifier, std::any symbol) {
    std::cout << "Identifier: " << identifier
    << "\tValue: " << std::any_cast<double>(symbol) 
    << std::endl;
}

bool Number::changeExpressionToDeclaredTypeIfItIsLegal(AST* node, int line, ContextManager* contextManager) {
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
        if (!changeExpressionToDeclaredTypeIfItIsLegal(child, line, contextManager)) {
            return false;
        }
    }
    return true;
}