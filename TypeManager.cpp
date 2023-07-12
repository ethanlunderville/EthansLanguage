#include "TypeManager.h"

TypeManager::TypeManager(){}
TypeManager::~TypeManager(){}

Type* TypeManager::getTypeHandler(std::string type){
    return this->stringToTypeHandler[type];
}

Type* TypeManager::getTypeHandler(TokenType type){
    return this->tokenToTypeHandler[type];
}

void TypeManager::addTypeDecl(std::string sString, TokenType token, Type* typeHandle) { 
    this->stringToTypeHandler[sString] = typeHandle;
    this->tokenToTypeHandler[token] = typeHandle;
    this->Declarator.push_back(token);
} 

void TypeManager::addTypeRVal(TokenType token, Type* typeHandle) { 
    this->tokenToTypeHandler[token] = typeHandle;
    this->Data.push_back(token);
} 

bool TypeManager::tokenIsRValue(TokenType tokenType) {
    if (this->tokenToTypeHandler.count(tokenType) > 0) {
        return true;
    }
    return false;
}
