#pragma once
#include "SymbolTable/Types/Types.h"
#include "Keywords.h"

/* MANAGES TYPES THROUGHOUT INTERPRETER */

class TypeManager {
    public:
        TypeManager();
        ~TypeManager();
        Type* getTypeHandler(const std::string& type);
        Type* getTypeHandler(TokenType type);
        void createType(const std::string& sString, Struct* typeHandle);
        void addTypeDecl(const std::string& sString, TokenType token, Type* typeHandle);
        void addTypeRVal(TokenType token, Type* typeHandle); 
        bool tokenIsRValue(TokenType tokenType);
        std::vector<TokenType> Data;
        std::vector<TokenType> Declarator;
    private:
        std::map<std::string ,Type*> stringToTypeHandler;
        std::map<TokenType, Type*> tokenToTypeHandler;
        std::vector<Struct*> holderForStructTypeDestructor;
};