#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AddTree::AddTree() : Operator() {
    this->setPrecedence(2);
}

std::any AddTree::add(std::any operand1, std::any operand2) {
    std::any res;
    if (typeid(std::string)==operand1.type() && typeid(std::string)==operand2.type()) {
        res = static_cast<std::string>(std::any_cast<std::string>(operand1) + std::any_cast<std::string>(operand2));
    } else if (typeid(std::string)==operand1.type() && typeid(double)==operand2.type()) {
        res = static_cast<std::string>(std::any_cast<std::string>(operand1) + std::to_string(std::any_cast<double>(operand2)));
    } else if (typeid(double)==operand1.type() && typeid(std::string)==operand2.type()) {
        res = static_cast<std::string>(std::to_string(std::any_cast<double>(operand1)) + std::any_cast<std::string>(operand2));
    } else if (typeid(double)==operand1.type() && typeid(double)==operand2.type()) {
        res = static_cast<double>(std::any_cast<double>(operand1) + std::any_cast<double>(operand2));
    } else if (typeid(std::vector<std::any>)==operand1.type() && typeid(std::vector<std::any>)==operand2.type()) {
        std::vector<std::any> firstVec = std::any_cast<std::vector<std::any>>(operand1);
        std::vector<std::any> secondVec = std::any_cast<std::vector<std::any>>(operand2);
        for (int i = 0 ; i < secondVec.size() ; i++) { firstVec.push_back(secondVec[i]); }
        res = firstVec;
    } else if (typeid(std::vector<std::any>)==operand2.type()) {
        std::vector<std::any> anyVec = std::any_cast<std::vector<std::any>>(operand2);
        anyVec.insert(anyVec.begin(), operand1);
        res = anyVec;
    } else if (typeid(std::vector<std::any>)==operand1.type() ) {
        std::vector<std::any> anyVec = std::any_cast<std::vector<std::any>>(operand1);
        anyVec.push_back(operand2);
        res = anyVec;
    } 
    this->setVal(res);
    return res;
}

void AddTree::accept(ASTVisitor* v) { v->visitAddTree(this); }