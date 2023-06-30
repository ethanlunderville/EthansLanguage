#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H

#include "SymbolTable.h"
#include <iostream>
#include <stack>
#include <any>
#include <string>

class ContextManager {
public:
    ContextManager();
    ~ContextManager();
    void pushContext();
    void popContext();
    void pushScope();
    void popScope();
    void declareSymbol(int line, std::string identifier, std::any value, std::string type);
    void reassignSymbol(std::string identifier, std::any value, int line);
    void printSymbolTable();
    std::any getValueStoredInSymbol(std::string identifier);
    std::string getTypeOfSymbol(std::string identifier);
private:
    SymbolTable* globalContext;
    std::stack<SymbolTable*> contextStack;
};

#endif
