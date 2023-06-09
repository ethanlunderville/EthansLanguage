#include "AST.h"

class DeclarationTree : public AST {
    public:
        DeclarationTree() {}
        void accept(ASTVisitor v) {}
} {}