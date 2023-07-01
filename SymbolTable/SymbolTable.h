#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <string>
#include <any>
#include <stack>
#include <vector>
#include <map>

typedef struct SymbolInfo {
    int line;
    std::any value;
    std::string type;
} SymbolInfo;

/*
* Unopinionated SymbolTable
*/
class SymbolTable {
public:
    SymbolTable(SymbolTable* tableReference);

    void pushScope();
    void popScope();
    void declareSymbol(int line, std::string identifier, std::string type);
    void reassignSymbol(std::string identifier, std::any value);
    std::string getTypeOfSymbol(std::string identifier);
    std::any getValueStoredInSymbol(std::string identifier);
    int getCurrentSize();
    int contains(std::string identifier);
    void printSymbolTable();
    bool variableTypeCheck(std::string type , std::any value);

private:
    SymbolTable* globalTable;
    std::stack<int> scopeStack;
    std::vector<std::string> intToStringVector;
    std::map<std::string, SymbolInfo> stringToSymbolMap;
};

#endif
