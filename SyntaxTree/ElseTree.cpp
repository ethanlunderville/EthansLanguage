#include "AST.h"

class ElseTree : public AST {
    public:
        ElseTree() {}
        void accept(ASTVisitor v) {}
};