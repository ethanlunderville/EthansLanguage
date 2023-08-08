
#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include "SymbolTable/ContextManager.h"

Assignable::Assignable() : type(nullptr) {
}
Assignable::~Assignable() {}

int Assignable::getLine() {
    return this->line; 
}

void Assignable::setLine(int line ) {
    this->line = line;
}

void Assignable::setCheckerReference(ASTChecker* checkerReference) {
    this->checkerReference = checkerReference;
} 

void Assignable::setType(Type* type) {
    this->type = type;
}

Type* Assignable::getType() {
    return this->type;
}

ASTChecker* Assignable::getCheckerReference () { return this->checkerReference; }