#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AddTree::AddTree() : Operator() {
    this->setPrecedence(2);
}

std::string AddTree::add(double x, std::string y) {
    std::string res = std::to_string(x) + y;
    this->setVal(res);
    return res;
}

std::string AddTree::add(std::string x, double y) {
    std::string res = x + std::to_string(y);
    this->setVal(res);
    return res;
}

std::string AddTree::add(std::string x, std::string y) {
    std::string res = x+y;
    this->setVal(res);
    return res;
}

double AddTree::add(double x, double y) {
    double res = x + y;
    this->setVal(res);
    return res; 
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
    }
    this->setVal(res);
    return res;
}

void AddTree::accept(ASTVisitor* v) { v->visitAddTree(this); }