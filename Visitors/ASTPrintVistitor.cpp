#include <cmath>
#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

//#define PRINTNODES (0x0)

ASTPrintVisitor::ASTPrintVisitor(){
    this->indent = 1;
    this->lineNum = 1;
}

void ASTPrintVisitor::visitChildren(AST* astree){
    for (AST* child : astree->getChildren()) {
        (child)->accept(this);
    }
}

void ASTPrintVisitor::printIndent() {
    /*
    * This is so the tree stays consisted despite
    * the size of the number printed before each 
    * node.
    */
    if (std::log10(this->lineNum) == std::floor(std::log10(this->lineNum))) {
        if ( (this->indent - 1) > 0) {
            this->indent--;
        }
    }
    for (int i = 0 ; i < this->indent ; i++) {
        std::cout << " ";
    }
}

void ASTPrintVisitor::printer(std::string type, AST* node, std::string symbol) {
    std::cout << this->lineNum << ".";
    printIndent();
    std::cout << type << ": " << symbol; 
    #ifdef PRINTNODES
        std::cout << ": " << node;
    #endif
    std::cout << std::endl;
    this->indent+=2;
    this->lineNum++;
    this->visitChildren(node);
    this->indent-=2;
}

void ASTPrintVisitor::printer(std::string type, AST* node) {
    std::cout << this->lineNum << ".";
    printIndent();
    std::cout << type;
    #ifdef PRINTNODES 
        std::cout << ": " << node; 
    #endif
    std::cout << std::endl;
    this->indent+=2;
    this->lineNum++;
    this->visitChildren(node);
    this->indent-=2;
}

void ASTPrintVisitor::visitAssignTree (AST* astree) { printer("AssignTree", astree, ((AssignTree*)astree)->getIdentifier());}
void ASTPrintVisitor::visitFunctionAssignTree (AST* astree) { printer("FunctionAssignTree", astree, ((FunctionAssignTree *)astree)->getIdentifier());}
void ASTPrintVisitor::visitArrayAssignTree (AST* astree) { printer("ArrayAssignTree", astree, ((ArrayAssignTree*)astree)->getIdentifier());}
void ASTPrintVisitor::visitStructAssignTree (AST* astree) { printer("StructAssignTree", astree, ((StructAssignTree*)astree)->getIdentifier());}
void ASTPrintVisitor::visitIfTree (AST* astree) {printer("IfTree", astree);}
void ASTPrintVisitor::visitExpressionTree (AST* astree) {printer("ExpressionTree", astree);}
void ASTPrintVisitor::visitStringExpressionTree (AST* astree) {printer("StringExpressionTree", astree);}
void ASTPrintVisitor::visitDivideTree (AST* astree) {printer("DivideTree", astree, "/");}
void ASTPrintVisitor::visitMultiplyTree (AST* astree) {printer("MultiplyTree", astree,"x");}
void ASTPrintVisitor::visitAddTree (AST* astree) {printer("AddTree", astree, "+");}
void ASTPrintVisitor::visitSubtractTree (AST* astree) {printer("SubtractTree", astree, "-");}
void ASTPrintVisitor::visitExponentTree (AST* astree) {printer("ExponentTree", astree, "^");}
void ASTPrintVisitor::visitDeclarationTree (AST* astree) {printer("DeclarationTree", astree, ((DeclarationTree*)astree)->getIdentifier());}
void ASTPrintVisitor::visitBlockTree (AST* astree) {printer("BlockTree", astree);}
void ASTPrintVisitor::visitReturnTree (AST* astree) {printer("ReturnTree", astree);}
void ASTPrintVisitor::visitProgramTree (AST* astree) {printer("ProgramTree", astree);}
void ASTPrintVisitor::visitFunctionDeclarationTree (AST* astree) {printer("FunctionDeclarationTree", astree, ((FunctionDeclarationTree*)astree)->getIdentifier());}
void ASTPrintVisitor::visitArrayDeclarationTree (AST* astree) {printer("ArrayDeclarationTree", astree, ((ArrayDeclarationTree*)astree)->getIdentifier());}
void ASTPrintVisitor::visitStructDeclarationTree (AST* astree) {printer("StructDeclarationTree", astree, ((StructDeclarationTree*)astree)->getIdentifier());}
void ASTPrintVisitor::visitFunctionCallTree (AST* astree) {printer("FunctionCallTree", astree, ((FunctionCallTree*)astree)->getIdentifier() );}
void ASTPrintVisitor::visitWhileTree (AST* astree) {printer("WhileTree", astree);}
void ASTPrintVisitor::visitElseTree (AST* astree) {printer("ElseTree", astree);}
void ASTPrintVisitor::visitNumberTree (AST* astree) {printer("NumberTree", astree, std::to_string(std::any_cast<double>(((NumberTree*)astree)->getVal())));}
void ASTPrintVisitor::visitStringTree (AST* astree) {printer("StringTree", astree, std::any_cast<std::string>(((StringTree*)astree)->getVal()));}
void ASTPrintVisitor::visitIdentifierTree (AST* astree) {
    IdentifierTree* t = ((IdentifierTree*)astree);
    printer("Identifier",astree, t->getIdentifier());
    //if (t->getSubscript() != nullptr) {
   //     printIndent();
   // }
}
void ASTPrintVisitor::visitGreaterTree (AST* astree) {printer("GreaterTree", astree, ">");}
void ASTPrintVisitor::visitGreaterEqualTree (AST* astree) {printer("GreaterEqualTree", astree, ">=");}
void ASTPrintVisitor::visitLessTree (AST* astree) {printer("LessTree", astree, "<");}
void ASTPrintVisitor::visitLessEqualTree (AST* astree) {printer("LessEqualTree", astree, "<");}
void ASTPrintVisitor::visitEqualTree (AST* astree) {printer("EqualTree", astree, "==");}
void ASTPrintVisitor::visitNotEqualTree (AST* astree) {printer("NotEqualTree", astree, "!=");}
void ASTPrintVisitor::visitAndTree (AST* astree) {printer("AndTree", astree, "&&");}
void ASTPrintVisitor::visitOrTree (AST* astree) {printer("OrTree", astree, "||");}
void ASTPrintVisitor::visitAssignOpTree (AST* astree) {printer("AssignOpTree", astree, "=");}
void ASTPrintVisitor::visitArrowOpTree (AST* astree) {printer("ArrowOpTree", astree, "->");}

