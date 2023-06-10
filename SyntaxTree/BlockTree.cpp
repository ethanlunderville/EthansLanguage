#include "AST.h"

class BlockTree : public AST {
    public:
        BlockTree() {}
        void accept(ASTVisitor v) {}
};