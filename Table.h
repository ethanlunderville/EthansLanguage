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

class SymbolTable {
public:
    SymbolTable(SymbolTable* tableReference);

    void pushScope();
    void popScope();
    void declareSymbol(int line, std::string identifier, std::any value, std::string type);
    void reassignSymbol(std::string identifier, std::any value);
    std::string getTypeOfSymbol(std::string identifier);
    auto getValueStoredInSymbol(std::string identifier);
    int getCurrentSize();
    int contains(std::string identifier);
    void printSymbolTable();
    void printSymbolTable(int continuationIndex);

private:
    struct SymbolInfo {
        int line;
        std::any value;
        std::string type;
    };

    SymbolTable* globalTable;
    std::stack<int> scopeStack;
    std::vector<std::string> intToStringVector;
    std::map<std::string, SymbolInfo> stringToSymbolMap;
};

#endif  //TABLE_H
