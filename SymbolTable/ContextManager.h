#pragma once
#include "SymbolTable.h"
#include "TypeManager.h"
#include <stack>
#include <string>

class Type;
class ContextManager {
public:
    ContextManager(TypeManager* typeManager);
    ~ContextManager();
    void pushContext();
    void popContext();
    void pushScope();
    void popScope();
    void declareSymbol(int line, std::string identifier, std::string type);
    void reassignSymbol(std::string identifier, std::any value, int line);
    void contextPopRecurse(SymbolTable* sym);
    void printSymbolTable();
    std::any getValueStoredInSymbol(std::string identifier);
    Type* getTypeOfSymbol(std::string identifier);
private:
    SymbolTable* globalContext;
    SymbolTable* globalScopeLinkedList;
    std::stack<SymbolTable*> contextStack;
    TypeManager* typeManager;
};
