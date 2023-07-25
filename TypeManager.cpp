#include "TypeManager.h"

TypeManager::TypeManager(){}
TypeManager::~TypeManager(){
    for (int i = 0 ; i < this->holderForStructTypeDestructor.size() ; i++) {
        delete this->holderForStructTypeDestructor[i];
    }
}

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

void TypeManager::createType(std::string sString, Struct* typeHandle) {
    if (this->stringToTypeHandler.count(sString) > 0) {
        this->stringToTypeHandler[sString] = typeHandle;
        this->holderForStructTypeDestructor.push_back(typeHandle);
    }
}

bool TypeManager::tokenIsRValue(TokenType tokenType) {
    if (this->tokenToTypeHandler.count(tokenType) > 0) {
        return true;
    }
    return false;
}
