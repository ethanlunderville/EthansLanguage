#include "SymbolTable/Types/Types.h"
#include "Keywords.h"
#pragma once

class TypeManager {
    public:
        TypeManager();
        ~TypeManager();
        Type* getTypeHandler(std::string type);
        Type* getTypeHandler(TokenType type);
    private:
        std::map<std::string, Type*> stringToTypeHandler;
        std::map<TokenType, Type*> tokenToTypeHandler;
};

static TokenType Data[] = {
    STRING,NUMBER
};

static TokenType Declarator[] = {
    STRINGTYPE,INT
};