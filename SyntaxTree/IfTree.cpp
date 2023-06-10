#include "AST.h"

class IfTree : public AST {
    public:
        IfTree() {}
        void accept(ASTVisitor v) {}
};