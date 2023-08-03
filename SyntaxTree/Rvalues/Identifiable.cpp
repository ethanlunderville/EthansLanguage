#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

Identifiable::~Identifiable(){}

std::string Identifiable::getIdentifier() {
    return this->identifier;
} 
void Identifiable::setIdentifier(const std::string& identifier) {
    this->identifier = identifier;
}