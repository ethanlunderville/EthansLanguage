#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AssignOpTree::AssignOpTree() : Operator() {
    this->setPrecedence(0);
}

void AssignOpTree::assign(std::any* x, std::any y) {
    std::any left = *x;
    if (left.type() == typeid(char*) && y.type() == typeid(std::string)) {
        char* xPointer = std::any_cast<char*>(*x);
        std::string potentialChar = std::any_cast<std::string>(y);
        if (potentialChar.size() == 1) {
            (*xPointer) = potentialChar[0];
            return;
        }
        std::cerr << "only single character strings may be assigned to string subscripts";
        exit(1);
    }
    (*x) = y;
}

void AssignOpTree::accept(ASTVisitor* v) { v->visitAssignOpTree(this); }