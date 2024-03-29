#pragma once
#include <any>
#include <string>
#include <iostream>
#include "SyntaxTree/AST.h"

/*
    THESE ARE THE CLASSES PERTAINING TO THE LANGUAGES
    TYPES INSTANCES ARE ASSIGNED TO ENTRIES IN THE SYMBOLTABLE
*/

class ContextManager;
class SymbolTable;

class Type {
    public:
        virtual ~Type() {}
        virtual std::any getNullValue()= 0;
        virtual void printType()= 0;
        virtual void printSymbol(std::string identifier, std::any symbol)= 0;
        virtual void printArrayOfType(std::any vector)= 0;
        virtual bool checkType(std::any value) = 0;
};

class PrimitiveType : public Type {
    public:
        virtual AST* getNewTreenode(std::string value) = 0;
};

class Struct : public Type {
    public:
        Struct(SymbolTable* baseStruct, const std::string& identifier);
        Struct(const std::string& identifier);
        std::any getNullValue() override;
        void printSymbol(std::string identifier, std::any symbol) override;
        void printSymbol(std::any symbol);
        bool checkType(std::any type) override;
        void printArrayOfType(std::any vector) override;
        void printType() override;
        SymbolTable* getDuplicateBase();
        SymbolTable* getBaseStructPointer();
        void setBaseStructPointer(SymbolTable* base);
    private:
        std::string structIdentifier;
        SymbolTable* baseStruct;
};

class Number : public PrimitiveType {
    public:
        std::any getNullValue() override;
        AST* getNewTreenode(std::string value) override;
        void printSymbol(std::string identifier, std::any symbol) override;
        bool checkType(std::any type) override;
        void printArrayOfType(std::any vector) override;
        void printType() override;
        static Number* getInstance() {
            static Number instance;
            return &instance;
        }  
    private:
        Number();  
};
class String : public PrimitiveType {
    public:
        std::any getNullValue() override;
        AST* getNewTreenode(std::string value) override;
        void printSymbol(std::string identifier, std::any symbol) override;
        bool checkType(std::any type) override;
        void printArrayOfType(std::any vector) override;
        void printType() override;
        static String* getInstance() {
            static String instance;
            return &instance;
        }
    private:
        String();
};

class Function : public Type {
    public:
        Function(Type* returnType);
        ~Function();
        std::any getNullValue() override;
        void printSymbol(std::string identifier, std::any symbol) override;
        bool checkType(std::any type) override;
        Type* getFunctionType();
        void printArrayOfType(std::any vector) override;
        void printType() override;
        Type* type;
};

class Array : public Type {
    public:
        Array(Type* arrayType);
        ~Array();
        std::any getNullValue() override;
        void printSymbol(std::string identifier, std::any symbol) override;
        bool checkType(std::any type) override;
        Type* getArrayType();
        void printArrayOfType(std::any vector) override;
        void printType() override;
        Type* type;
};

