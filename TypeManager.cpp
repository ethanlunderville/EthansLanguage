#include "TypeManager.h"

TypeManager::TypeManager(){
    String* stringType = new String();
    Number* numberType = new Number();
    this->stringToTypeHandler = {
        {std::string("string"), stringType},
        {std::string("int"), numberType}
    };
    this->tokenToTypeHandler = {
        {STRINGTYPE, stringType},
        {STRING, stringType},
        {INT, numberType},
        {NUMBER, numberType}
    };
}
TypeManager::~TypeManager(){
    delete this->stringToTypeHandler[std::string("string")];
    delete this->stringToTypeHandler[std::string("int")];
    this->stringToTypeHandler[std::string("string")] = nullptr;
    this->stringToTypeHandler[std::string("int")] = nullptr;
}
Type* TypeManager::getTypeHandler(std::string type){
    return this->stringToTypeHandler[type];
}
Type* TypeManager::getTypeHandler(TokenType type){
    return this->tokenToTypeHandler[type];
}