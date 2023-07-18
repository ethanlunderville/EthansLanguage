#pragma once
#include "SymbolTable/Types/Types.h"
#include "Keywords.h"

class TypeManager {
    public:
        TypeManager();
        ~TypeManager();
        PrimitiveType* getTypeHandler(std::string type);
        PrimitiveType* getTypeHandler(TokenType type);
        void addTypeDecl(std::string sString, TokenType token, PrimitiveType* typeHandle);
        void addTypeRVal(TokenType token, PrimitiveType* typeHandle); 
        bool tokenIsRValue(TokenType tokenType);
        std::vector<TokenType> Data;
        std::vector<TokenType> Declarator;
    private:
        std::map<std::string ,PrimitiveType*> stringToTypeHandler;
        std::map<TokenType, PrimitiveType*> tokenToTypeHandler;
};