#include "AST.h"

class ReturnTree : public AST {
    public:
        ReturnTree() {}
        void accept(ASTVisitor v) {}
} {}