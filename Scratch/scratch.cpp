#include <iostream>
#include <any>
#include <vector>
#include <map>
#include <string>
#include <regex>
#include <algorithm>
#include <functional>

struct Rule {
    Rule() {}
    Rule(std::string& rule, std::vector<Rule*>& ruleVec) : ruleString(rule) {
        ruleVec.push_back(this);
    }
    virtual ~Rule() {}
    std::string ruleString;
    std::string matched;
};
struct CompoundRule : public Rule {
    CompoundRule(std::string& rule, std::vector<Rule*>& ruleVec)  {
        if (typeid(*(ruleVec[ruleVec.size()-1])) != typeid(CompoundRule)) {
            this->childRule = ruleVec[ruleVec.size()-1];
            ruleVec[ruleVec.size()-1] = this;
            this->ruleString = rule;
        } else {
            std::cerr 
            << "Invalid regex :: two compound rules in a row" 
            << std::endl;
        }
    }
    ~CompoundRule() {
        delete this->childRule;
        childRule = nullptr;
    } 
    Rule* childRule;
};

void printVec(std::vector<Rule*>& ruleVector) {
    for (Rule* rule : ruleVector) {
        if (typeid(*rule) == typeid(CompoundRule)) {
            std::cout << "@" 
            << dynamic_cast<CompoundRule*>(rule)->childRule->ruleString 
            << "@ ";
        } 
        std::cout << rule->ruleString << std::endl;
    }
}

void deallocateRules(std::vector<Rule*>& ruleVector) {
    for (Rule* rule : ruleVector) {
        delete rule;
    }
}

int main () {
    std::string inputRegex = "[0-9]{3}";
    std::string inputString;
    std::vector<Rule*> ruleVector;
    int size = inputRegex.size();
    /**/
    auto peek = [&size, &inputRegex] (int * i) -> char {
        if ((*i) != size - 1) {
            return inputRegex[(*i)];
        }
        return '\0';
    };

    auto increment = [&size] (int * i) -> void {
        if ((*i) != size - 1) {
            (*i)++;
            return;
        }
        std::cerr 
        << "Invalid regular expression"
        << std::endl;
        exit(1);
    };
    /*PARSE REGEX*/
    for (int i = 0 ; i < size ; i++) {
        std::string spanRule = "";
        switch (inputRegex[i]) {
            case '[':
                while (inputRegex[i] != ']') {
                    spanRule.push_back(inputRegex[i]);
                    increment(&i);
                }
                spanRule.push_back(inputRegex[i]);
                ruleVector.push_back(new Rule(spanRule, ruleVector));
            break;
            case '{':
                while (inputRegex[i] != '}') {
                    spanRule.push_back(inputRegex[i]);
                    increment(&i);
                }
                spanRule.push_back(inputRegex[i]);
                ruleVector.push_back(new CompoundRule(spanRule, ruleVector));
            break;
        }
    }
    printVec(ruleVector);
    deallocateRules(ruleVector);
    return 0;
}