#include "SyntaxTree/AST.h"

class DivideTree : public AST {
    public:
        DivideTree() {}
        void accept(ASTVisitor v) {}
};