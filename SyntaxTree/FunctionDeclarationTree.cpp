#include "AST.h"

class FunctionDeclarationTree : public AST {
    public:
        FunctionDeclarationTree(std::string* name) {
            this->name = name;
        }
        void accept(ASTVisitor v) {}
    public:
        std::string* name;
};