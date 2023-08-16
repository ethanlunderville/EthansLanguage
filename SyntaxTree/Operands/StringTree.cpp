#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include <algorithm>

static std::map<char, char> escapeMap = {
    {'n','\n'}, 
    {'t','\t'}
};

StringTree::StringTree(std::string sString) : Evaluatable() {

    auto checkForEscape = [] (char c) -> bool {
        if (escapeMap.find(c) != escapeMap.end()) {
            return true;
        }
        return false;
    };
    
    auto getEscape = [] (char c) -> char {
        return escapeMap[c];
    };

    char quote = '\"'; 
    sString.erase(
        std::remove(sString.begin(), sString.end(), quote), 
        sString.end()
    );
    this->sString = sString.substr(0, sString.size() - 1);
    std::string replaced;
    for (size_t i = 0; i < this->sString.size(); ++i) {
        if (i + 1 < this->sString.size() && 
            this->sString[i] == '\\' && 
            checkForEscape(this->sString[i + 1])
        ) {
            replaced += getEscape(this->sString[i + 1]);
            ++i;
        } else {
            replaced += this->sString[i];
        }
    }
    this->sString = replaced;
    this->setVal(this->sString);
}

std::string StringTree::getString() {
    return this->sString;
}

void StringTree::accept(ASTVisitor* v) { v->visitStringTree(this); }