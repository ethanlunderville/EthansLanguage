#include "SyntaxTree/AST.h"

class SubtractTree : public AST {
    public:
        SubtractTree() {}
        void accept(ASTVisitor v) {}
} {}