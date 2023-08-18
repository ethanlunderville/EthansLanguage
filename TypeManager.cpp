#include "TypeManager.h"

TypeManager::TypeManager(){}

TypeManager::~TypeManager(){
    for (int i = 0 ; i < this->holderForStructTypeDestructor.size() ; i++) {
        delete this->holderForStructTypeDestructor[i];
    }
}

Type* TypeManager::getTypeHandler(const std::string& type){
    Type* t = this->stringToTypeHandler[type];
    if (t != nullptr) {
        return t;
    }
    std::cerr << "Unrecognized type: " << type << std::endl;
    exit(1);
}

Type* TypeManager::getTypeHandler(Tokens type){
    return this->tokenToTypeHandler[type];
}

void TypeManager::addTypeDecl(const std::string& sString, Tokens token, Type* typeHandle) { 
    this->stringToTypeHandler[sString] = typeHandle;
    this->tokenToTypeHandler[token] = typeHandle;
    this->Declarator.push_back(token);
} 

void TypeManager::addTypeRVal(Tokens token, Type* typeHandle) { 
    this->tokenToTypeHandler[token] = typeHandle;
    this->Data.push_back(token);
} 

void TypeManager::createType(const std::string& sString, Struct* typeHandle) {
    if (this->stringToTypeHandler.count(sString) == 0) {
        this->stringToTypeHandler[sString] = typeHandle;
        this->holderForStructTypeDestructor.push_back(typeHandle);
    } else {
        std::cerr << "Invalid attempt to redeclare struct." << std::endl;
        exit(1);
    }
}

bool TypeManager::tokenIsRValue(Tokens token) {
    if (this->tokenToTypeHandler.count(token) > 0) {
        return true;
    }
    return false;
}
