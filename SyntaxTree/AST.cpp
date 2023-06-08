#include <vector>
#include "AST.h"
class AST {

    public:

        AST() {

        }

        void addChild(AST* tree) {
            children.push_back(tree);
        }

        std::vector<AST*> getChildren() {
            return children;
        }

        void* accept(ASTVisitor v);

    private:

        std::vector<AST*> children;

};