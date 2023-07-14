#pragma once

#include "SymbolTable/Types/Types.h"
#include <iostream>
#include <string>
#include <any>
#include <stack>
#include <vector>
#include <map>

struct SymbolInfo {
    int line;
    std::any value;
    Type* type;
} ;

/*
* Unopinionated SymbolTable
*/
class SymbolTable {
public:
    SymbolTable(SymbolTable* tableReference);
    void pushScope();
    void popScope();
    void declareSymbol(int line, std::string identifier, Type* typeHandler);
    void reassignSymbol(std::string identifier, std::any value);
    Type* getTypeOfSymbol(std::string identifier);
    std::any getValueStoredInSymbol(std::string identifier);
    int getCurrentSize();
    int contains(std::string identifier);
    void printSymbolTable();
    bool variableTypeCheck(Type* typeHandler , std::any value);
    SymbolTable* tableReference;
private:
    std::stack<int> scopeStack;
    std::vector<std::string> intToStringVector;
    std::map<std::string, SymbolInfo> stringToSymbolMap;
};
