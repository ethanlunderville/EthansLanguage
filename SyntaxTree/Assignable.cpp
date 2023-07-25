
#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include "SymbolTable/ContextManager.h"
Assignable::Assignable() {}
Assignable::~Assignable() {}
int Assignable::getLine() { return this->line; }
void Assignable::setLine(int line ) { this->line = line;}
void Assignable::setCheckerReference(ContextManager* checkerReference) { this->checkerReference = checkerReference; } 
ContextManager* Assignable::getCheckerReference () { return this->checkerReference; }