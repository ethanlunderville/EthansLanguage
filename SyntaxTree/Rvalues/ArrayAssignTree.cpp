#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include "SymbolTable/ContextManager.h"

ArrayAssignTree::ArrayAssignTree(std::string identifier, int line) {
    this->identifier = identifier;
    this->setLine(line);
}

std::string ArrayAssignTree::getIdentifier() {
    return this->identifier;
}

void ArrayAssignTree::accept(ASTVisitor* v) { v->visitArrayAssignTree(this); }