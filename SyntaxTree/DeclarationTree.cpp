#include "AST.h"

class DeclarationTree : public AST {
    public:
        DeclarationTree(std::string* name) {
            this->name = name;
        }
        void accept(ASTVisitor v) {}
    public:
        std::string* name;
};