#include "SyntaxTree/AST.h"

class AddTree : public AST {
    public:
        AddTree() {}
        void accept(ASTVisitor v) {}
} {}