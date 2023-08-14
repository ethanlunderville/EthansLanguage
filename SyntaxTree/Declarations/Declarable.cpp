#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

Declarable::~Declarable(){}

Declarable::Declarable(std::string type, std::string identifier, int line)
: identifier(identifier),
  type(type),
  line(line) {}

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