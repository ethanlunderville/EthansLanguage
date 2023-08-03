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
    SymbolTable* popScope(bool isStruct);
    void declarePrimitiveSymbol(int line, std::string identifier, std::string type);
    void declareSymbol(int line, std::string identifier, Type* type);
    void declareSymbol(int, std::string, std::string);
    void reassignSymbol(std::string identifier, std::any value, int line);
    void contextPopRecurse(SymbolTable* sym);
    void printSymbolTable();
    std::any getValueStoredInSymbol(std::string identifier);
    std::any* getReferenceOfValueStoredInSymbol(std::string identifier);
    std::any getValueStoredInSymbol(std::string identifier, int subscript);
    std::any* getReferenceOfValueStoredInSymbol(std::string identifier, int subscript);
    Type* getTypeOfSymbol(std::string identifier);
    void setCurrentFunctionType(Type* t);
    Type* getCurrentFunctionType();
private:
    SymbolTable* globalContext;
    SymbolTable* globalScopeLinkedList;
    std::stack<SymbolTable*> contextStack;
    TypeManager* typeManager;
};
