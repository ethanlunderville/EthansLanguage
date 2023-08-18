#pragma once
#include "SymbolTable/Types/Types.h"
#include "Keywords.h"

/* MANAGES TYPES THROUGHOUT INTERPRETER */

class TypeManager {
    public:
        TypeManager();
        ~TypeManager();
        Type* getTypeHandler(const std::string& type);
        Type* getTypeHandler(Tokens type);
        void createType(const std::string& sString, Struct* typeHandle);
        void addTypeDecl(const std::string& sString, Tokens token, Type* typeHandle);
        void addTypeRVal(Tokens token, Type* typeHandle); 
        bool tokenIsRValue(Tokens token);
        std::vector<Tokens> Data;
        std::vector<Tokens> Declarator;
    private:
        std::map<std::string ,Type*> stringToTypeHandler;
        std::map<Tokens, Type*> tokenToTypeHandler;
        std::vector<Struct*> holderForStructTypeDestructor;
};