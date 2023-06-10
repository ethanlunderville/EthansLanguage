#include "AST.h"

class WhileTree : public AST {
    public:
        WhileTree() {}
        void accept(ASTVisitor v) {}
};