#include "AST.h"

class ExpressionTree : public AST {
    public:
        ExpressionTree() {}
        void accept(ASTVisitor v) {}
} {}