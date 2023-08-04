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
    ~SymbolTable();
    void pushScope();
    void popScope();
    void declareSymbol(int line, std::string identifier, Type* typeHandler);
    void reassignSymbol(std::string identifier, std::any value);
    Type* getTypeOfSymbol(std::string identifier);
    std::any getValueStoredInSymbol(std::string identifier);
    std::any* getReferenceOfValueStoredInSymbol(std::string identifier);
    std::any getValueStoredInSymbol(std::string identifier, int subscript);
    std::any* getReferenceOfValueStoredInSymbol(std::string identifier, int subscript);
    int getCurrentSize();
    int contains(std::string identifier);
    void printSymbolTable();
    bool variableTypeCheck(Type* typeHandler , std::any value);
    void setCurrentFunctionType(Type* t);
    Type* getCurrentFunctionType();
    std::string& getTypeName();
    void setTypeName(const std::string& name);
    std::string& getStructName();
    void setStructName(const std::string& name);
    int getReferenceCount();
    void incrementReferenceCount();
    void decrementReferenceCount();
    Type* functionType;
    SymbolTable* tableReference;
    std::vector<std::string> intToStringVector;
    std::map<std::string, SymbolInfo> stringToSymbolMap;
    std::string typeName;
    std::string structName;
    int referenceCount;
    bool returned;
private:
    std::stack<int> scopeStack;
    std::vector<Type*> deletableTypes; 
    std::vector<SymbolTable*> structs; 
};
