#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

NotEqualTree::NotEqualTree() : Operator() {
    this->setPrecedence(1);
}

std::any NotEqualTree::notEqual(std::any x, std::any y) {
    if (x.type() == typeid(std::string) && y.type() == typeid(std::string)) {
        if (std::any_cast<std::string>(x).compare(std::any_cast<std::string>(y)) != 0) {
            this->setVal(1.00);
            return 1;
        } 
        this->setVal(0.00); 
        return 0;
    } else if (x.type() == typeid(double) && y.type() == typeid(double)) {
        if (std::any_cast<double>(x) != std::any_cast<double>(y)) {
            this->setVal(1.00);
            return 1;
        } 
        this->setVal(0.00);
        return 0;
    } else {
        std::cerr << "Uncomparable types" << std::endl;
        exit(1);
    }
}

void NotEqualTree::accept(ASTVisitor* v) { v->visitNotEqualTree(this); }