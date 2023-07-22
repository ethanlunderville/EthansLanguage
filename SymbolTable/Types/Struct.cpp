#include "Types.h"
#include "SymbolTable/ContextManager.h"

std::any Struct::getNullValue() {return nullptr;}
bool Struct::checkType(std::any value) {return true;}
void Struct::printSymbol(std::string identifier, std::any symbol) {}