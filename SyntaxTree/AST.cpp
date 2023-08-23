#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

AST::AST() {}

AST::~AST() {}

void AST::addChild(AST* tree) {
    children.push_back(tree);
}

int AST::getLine() {
    return this->line;
}

void AST::setLine(int line) {
    this->line = line;
}

std::vector<AST*>& AST::getChildren() {
    return children;
}

void AST::prependToChildren(AST* prependNode) {
    this->children.insert(this->children.begin(), prependNode);
}
