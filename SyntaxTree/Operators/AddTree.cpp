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

void AddTree::accept(ASTVisitor* v) { v->visitAddTree(this); }