#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

EqualTree::EqualTree() : Operator() {
    this->setPrecedence(1);
}

std::any EqualTree::equal(std::any x, std::any y) {

    if (x.type() == typeid(std::string) && y.type() == typeid(std::string)) {
        if (std::any_cast<std::string>(x).compare(std::any_cast<std::string>(y)) == 0) {
            this->setVal(1);
            return 1;
        }
        this->setVal(0);
        return 0;
    } else if (x.type() == typeid(char) && y.type() == typeid(char)) {
        double res = std::any_cast<char>(x) == std::any_cast<char>(y);
        this->setVal(res);
    } else {  
        double res = std::any_cast<double>(x) == std::any_cast<double>(y);
        this->setVal(res);
    }
    return res; 
}

void EqualTree::accept(ASTVisitor* v) { v->visitEqualTree(this); }