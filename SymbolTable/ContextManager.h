#pragma once
#include "SymbolTable.h"
#include "TypeManager.h"
#include <stack>
#include <string>

/*
    Manages symbol tables. (Structs/Function Contexts)

    Scope is managed by a context stack for function calls 
    and a global context. these contexts each have a linked 
    list every node in this list is a scope in that context.
    for instance, the globalScopeLinkedList is for all
    scopes in the global context (if/while blocks). That
    being said, each function context has its own linked list
    for its own subscopes. 

    This class manages these linked lists and delegates what
    symbols should be declared where so that scoping is 
    maintained. 

*/

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
    bool isFunctionReturned();
    void setFunctionIsReturned(bool isReturned);
    void dumpState();
private:
    SymbolTable* globalContext;
    SymbolTable* globalScopeLinkedList;
    std::stack<SymbolTable*> contextStack;
    TypeManager* typeManager;
};
