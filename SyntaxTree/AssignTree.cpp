#include "AST.h"

class AssignTree : public AST {
    public:
        AssignTree(std::string* name, std::string* value) {
            this->name = name;
            this->value = value;
        }
        void accept(ASTVisitor v) {}
    private:
        std::string* name;
        std::string* value;
};