#include "Regex.h"
#include "SymbolTable/ContextManager.h"
#include "Visitors/ASTVisitor.h"
#include "SyntaxTree/AST.h"

CompoundRule::CompoundRule(std::string& rule, std::vector<Rule*>& ruleVec, std::map<std::string, Rule*>& ruleToObjectMap) {
    if (ruleVec.size() == 0)  {
        std::cerr 
        << "Invalid regex :: compound rule cannot be the first rule in a regex" 
        << std::endl;
        exit(1);
    } 
    if (typeid(*(ruleVec[ruleVec.size()-1])) != typeid(CompoundRule)) {
        this->childRule = ruleVec[ruleVec.size()-1];
        ruleVec[ruleVec.size()-1] = this;
        this->ruleString = rule;
        ruleToObjectMap[this->childRule->getRule()] = nullptr;
        ruleToObjectMap[this->getRule()] = this;
        return;
    } 
    std::cerr 
    << "Invalid regex :: two compound rules in a row" 
    << std::endl;
    exit(1);
} 

void CompoundRule::printer() {
    std::cout << this->childRule->ruleString << this->ruleString;
}

std::string CompoundRule::getRule() {
    return this->childRule->ruleString + this->ruleString;
}

CompoundRule::~CompoundRule() {} 