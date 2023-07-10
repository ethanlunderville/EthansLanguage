#pragma once
#include <any>
#include <string>
#include <map>
#include <iostream>
#include "SyntaxTree/AST.h"

class Type {
    public:
        virtual ~Type() = 0;
        virtual std::any getNullValue() = 0;
        virtual bool checkType(std::any value) = 0;
        virtual void printSymbol(std::string identifier, std::any symbol) = 0;
        virtual bool checkIfExpressionOfThisTypeIsValid(AST* node, int line) = 0;
};

class TypeManager {
    public:
        TypeManager();
        ~TypeManager();
        Type* getTypeHandler(std::string typeString);
    private:
        std::map<std::string, Type*> stringToTypeHandler;
};

class Number : public Type {
    public:
        Number();
        std::any getNullValue() override;
        bool checkType(std::any value) override;
        void printSymbol(std::string identifier, std::any symbol) override;
        bool checkIfExpressionOfThisTypeIsValid(AST* node, int line) override;
};
class String : public Type {
    public:
        String();
        std::any getNullValue() override;
        bool checkType(std::any value) override;
        void printSymbol(std::string identifier, std::any symbol) override;
        bool checkIfExpressionOfThisTypeIsValid(AST* node, int line) override;
};

