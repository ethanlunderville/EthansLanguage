#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

EqualTree::EqualTree() : Operator() {
    this->setPrecedence(1);
}

std::any EqualTree::equal(std::any x, std::any y) {
    double res;
    if (x.type() == typeid(std::string) && y.type() == typeid(std::string)) {
        if (std::any_cast<std::string>(x).compare(std::any_cast<std::string>(y)) == 0) {
            this->setVal(1.00);
            return 1;
        }
        this->setVal(0.00);
        return 0;
    } else if (x.type() == typeid(char) && y.type() == typeid(char)) {
        bool res = std::any_cast<char>(x) == std::any_cast<char>(y);
        this->setVal((double)res);
    } else {  
        bool res = std::any_cast<double>(x) == std::any_cast<double>(y);
        this->setVal((double)res);
    }
    return res; 
}

void EqualTree::accept(ASTVisitor* v) { v->visitEqualTree(this); }