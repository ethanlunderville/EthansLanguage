#include "AST.h"
/*
* First Node of the Syntax Tree 
*/

class ProgramTree : public AST {

    public:
        ProgramTree() {

        }

        void accept(ASTVisitor v) {
            v.visitProgramTree(this);
        }
        
};