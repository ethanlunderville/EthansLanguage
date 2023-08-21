#include "Regex.h"
#include "SymbolTable/ContextManager.h"
#include "Visitors/ASTVisitor.h"
#include "SyntaxTree/AST.h"

Rule::Rule() {}

Rule::Rule(std::string& rule) : ruleString(rule) {}

Rule::Rule(std::string& rule, std::vector<Rule*>& ruleVec) : ruleString(rule) {
    ruleVec.push_back(this);
}

void Rule::printer() {
    std::cout << this->ruleString;
}

std::string Rule::getRule() {
    return this->ruleString;
}

void Rule::printMatch() {
    this->printer();
    if (!this->matched.has_value()) {
        std::cerr 
        << " ERROR :: Attempt to print empty match" 
        << std::endl;
        exit(1);
    }
    if (this->matched.type() == typeid(std::string)) {
        std::cout << " ::: " << std::any_cast<std::string>(this->matched);
        return;
    }
    if (this->matched.type() == typeid(std::vector<std::any>)) {
        std::vector<std::any> anyVec = std::any_cast<std::vector<std::any>>(this->matched);
        std::cout << " ::: {";
        for (int i = 0 ; i < anyVec.size() ; i++) {
            std::cout 
            << std::any_cast<std::string>(anyVec[i]);
            if (i != anyVec.size() -1) {
                std::cout << ", ";
            } 
        }
        std::cout << "} ";
        return;
    } 
}

void Rule::appendToMatchVector(std::string match) {
    if (!this->matched.has_value()) {
        std::cerr 
        << "Cannot append to match vector if there is not at least one match" 
        << std::endl;
        exit(1);
    } else if (this->matched.type() == typeid(std::string)) {
        std::vector<std::any> newAny = {std::move(this->matched)};
        newAny.push_back(std::any(match));
        this->matched = std::any(std::move(newAny));
    }
    std::any_cast<std::vector<std::any>>(this->matched).push_back(std::move(std::any(match)));
}

void Rule::setMatch(std::string match) {
    this->matched = (std::move(std::any(match)));
}

Rule::~Rule() { }