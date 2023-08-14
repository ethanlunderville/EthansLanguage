#pragma once

#include "SymbolTable/Types/Types.h"
#include <iostream>
#include <string>
#include <any>
#include <stack>
#include <vector>
#include <map>

// This is what houses the actual value
struct SymbolInfo { 
    int line;
    std::any value;
    Type* type;
};

/*
    Can be a regular scope, function context, or a struct
    different classes will be made for each in the future
*/

class SymbolTable {
public:
    SymbolTable();
    ~SymbolTable();

    void declareSymbol(int line, std::string identifier, Type* typeHandler);
    void reassignSymbol(std::string identifier, std::any value);
    Type* getTypeOfSymbol(std::string identifier);
    /*r-values*/
    std::any getValueStoredInSymbol(std::string identifier);
    std::any getValueStoredInSymbol(std::string identifier, int subscript);
    /*l-values*/
    std::any* getReferenceOfValueStoredInSymbol(std::string identifier);
    std::any* getReferenceOfValueStoredInSymbol(std::string identifier, int subscript);
    int getCurrentSize();
    int contains(std::string identifier);
    void printSymbolTable();
    bool variableTypeCheck(Type* typeHandler , std::any value);
    
    void setCurrentFunctionType(Type* t);
    Type* getCurrentFunctionType();

    std::string& getTypeName();
    std::string& getStructName();
    void setTypeName(const std::string& name);
    void setStructName(const std::string& name);
    int getReferenceCount();
    void incrementReferenceCount();
    void decrementReferenceCount();

    SymbolTable* tableReference;
    bool returned; 
    std::vector<Type*> deletableTypes; 
    std::vector<SymbolTable*> structs;
    Type* functionType;
    std::vector<std::string> intToStringVector;
    std::map<std::string, SymbolInfo> stringToSymbolMap;
    std::string typeName;
    std::string structName;
    int referenceCount;
    
};

