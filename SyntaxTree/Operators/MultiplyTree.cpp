#include "SyntaxTree/AST.h"

class MultiplyTree : public AST {
    public:
        MultiplyTree() {}
        void accept(ASTVisitor v) {}
} {}