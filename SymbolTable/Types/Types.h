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
        virtual AST* getNewTreenode(std::string value) = 0;
        virtual AST* getExpressionNode() = 0;
        virtual bool checkType(std::any value) = 0;
        virtual void printSymbol(std::string identifier, std::any symbol) = 0;
        virtual bool checkIfExpressionOfThisTypeIsValid(AST* node, int line) = 0;
};

class Number : public Type {
    public:
        Number();
        std::any getNullValue() override;
        AST* getNewTreenode(std::string value) override;
        AST* getExpressionNode() override;
        bool checkType(std::any value) override;
        void printSymbol(std::string identifier, std::any symbol) override;
        bool checkIfExpressionOfThisTypeIsValid(AST* node, int line) override;
};
class String : public Type {
    public:
        String();
        std::any getNullValue() override;
        AST* getNewTreenode(std::string value) override;
        AST* getExpressionNode() override;
        bool checkType(std::any value) override;
        void printSymbol(std::string identifier, std::any symbol) override;
        bool checkIfExpressionOfThisTypeIsValid(AST* node, int line) override;
};

