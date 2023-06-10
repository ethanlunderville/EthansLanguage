#include "AST.h"

class FunctionTree : public AST {
    public:
        FunctionTree() {}
        void accept(ASTVisitor v) {}
};