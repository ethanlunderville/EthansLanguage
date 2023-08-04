#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

Declarable::~Declarable(){}

std::string Declarable::getIdentifier(){
    return this->identifier;
}
void Declarable::setIdentifier(std::string& identifier){
    this->identifier = identifier;
}
std::string Declarable::getType(){
    return this->type;
}
void Declarable::setType(std::string& type){
    this->type = type;
}
int Declarable::getLine() {
    return this->line;
}