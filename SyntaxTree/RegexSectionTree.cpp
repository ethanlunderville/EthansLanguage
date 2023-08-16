#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

RegexSectionTree::RegexSectionTree(std::string& regex) : regex(std::move(regex)) {}
std::string& RegexSectionTree::getRegex() {
    return this->regex;
}
void RegexSectionTree::accept(ASTVisitor* v) { v->visitRegexSectionTree(this); };