#pragma once
#include <any>
#include <string>
#include <iostream>
#include "SyntaxTree/AST.h"

class ContextManager;

class Type {
    public:
        virtual ~Type() = 0;
        virtual std::any getNullValue() = 0;
        virtual bool checkType(std::any value) = 0;
        virtual void printSymbol(std::string identifier, std::any symbol) = 0;
};

class PrimitiveType : public Type {
    public:
        virtual AST* getNewTreenode(std::string value) = 0;
        virtual AST* getExpressionNode() = 0;
        virtual bool changeExpressionToDeclaredTypeIfItIsLegal(AST* node, int line, ContextManager* contextManager) = 0;
};

class Struct : public Type {
    std::any getNullValue() override;
    bool checkType(std::any value) override;
    void printSymbol(std::string identifier, std::any symbol) override;
};

class Number : public PrimitiveType {
    public:
        std::any getNullValue() override;
        AST* getNewTreenode(std::string value) override;
        AST* getExpressionNode() override;
        bool checkType(std::any value) override;
        void printSymbol(std::string identifier, std::any symbol) override;
        bool changeExpressionToDeclaredTypeIfItIsLegal(AST* node, int line, ContextManager* contextManager) override;
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
        AST* getExpressionNode() override;
        bool checkType(std::any value) override;
        void printSymbol(std::string identifier, std::any symbol) override;
        bool changeExpressionToDeclaredTypeIfItIsLegal(AST* node, int line, ContextManager* contextManager) override;
        void checkSubExpressionForString(AST* node);
        bool numberCheck(AST* node, int line, ContextManager* contextManager);
        bool checkTree(AST* node, int line, ContextManager* contextManager);
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
        bool checkType(std::any value) override;
        Type* type;
};

class Array : public Type {
    public:
        Array(Type* arrayType);
        ~Array();
        std::any getNullValue() override;
        void printSymbol(std::string identifier, std::any symbol) override;
        bool checkType(std::any value) override;
        Type* type;
};

