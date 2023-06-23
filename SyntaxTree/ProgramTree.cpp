#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

/*
* First Node of the Syntax Tree 
*/
ProgramTree::ProgramTree(){}
void ProgramTree::accept(ASTVisitor* v) { v->visitProgramTree(this); }