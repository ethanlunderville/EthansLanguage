#include <iostream>
#include <any>
#include <vector>
#include <map>
#include <string>
#include <regex>
#include <algorithm>
#include <functional>
#include <any>

struct Rule {

    Rule() {}

    Rule(std::string& rule) : ruleString(rule) {}

    Rule(std::string& rule, std::vector<Rule*>& ruleVec) : ruleString(rule) {
        ruleVec.push_back(this);
    }

    virtual void printer() {
        std::cout << this->ruleString;
    }

    virtual std::string getRule() {
        return this->ruleString;
    }

    void printMatch() {
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

    void appendToMatchVector(std::string match) {
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

    void setMatch(std::string match) {
        this->matched = (std::move(std::any(match)));
    }

    virtual ~Rule() { std::cout << "deleting " << this << std::endl; }

    std::string ruleString;
    std::any matched;
    bool isChild;
};

struct CompoundRule : public Rule {

    CompoundRule(std::string& rule, std::vector<Rule*>& ruleVec, std::map<std::string, Rule*>& ruleToObjectMap) {
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

    void printer() override {
        std::cout 
        << "$" 
        << this->childRule->ruleString 
        << this->ruleString 
        << "$";
    }

    std::string getRule() override {
        return this->childRule->ruleString + this->ruleString;
    }

    ~CompoundRule() {} 

    Rule* childRule;
};

void printVec(std::vector<Rule*>& ruleVector) {
    for (Rule* rule : ruleVector) {
        rule->printer();
        std::cout << "\n";
    }
}

void deallocateRules(std::vector<Rule*>& ruleVector) {
    std::vector<Rule*> freedAlready;
    for (int i = 0 ; i < ruleVector.size() ; i++) {
        if ( ruleVector[i] != nullptr && 
            std::find(freedAlready.begin(), freedAlready.end(), ruleVector[i]) == freedAlready.end() 
        ) { 
            if (typeid(*ruleVector[i]) == typeid(CompoundRule)) {
                Rule* temp = dynamic_cast<CompoundRule*>(ruleVector[i])->childRule;
                if (std::find(freedAlready.begin(), freedAlready.end(), temp) == freedAlready.end()) {
                    freedAlready.push_back(temp);
                    delete temp;
                }
            }
            freedAlready.push_back(ruleVector[i]);
            delete ruleVector[i];
        }
    }
}

std::string buildRegex(std::vector<Rule*>& ruleVector) {
    std::string regex;
    int size = ruleVector.size();
    for (int i = 0 ; i < size ; i++) {
        if (ruleVector[i] != nullptr) {
            regex.push_back('(');
            regex += ruleVector[i]->getRule();
            regex.push_back(')');
        }
    }
    return regex;
}

void printMatches(std::vector<Rule*>& ruleVector) {
    for (int i = 0 ; i < ruleVector.size() ; i++) {
        ruleVector[i]->printMatch();
        std::cout << "\n";
    }
}

int breakPoint() {return 1;}

int main () {

    std::string inputRegex = ".*";
    std::string input = "Contact us at 123-456-7890 or 987-654-3210 or 854-668-9090";
    std::vector<Rule*> ruleVector;
    std::map<std::string, Rule*> ruleToObjectMap;

    int size = inputRegex.size();
    
    auto getRuleObject = [&ruleToObjectMap] (std::string rule) -> Rule* {
        return ruleToObjectMap[rule];
    };

    auto checkRuleMap = [&ruleToObjectMap] (std::string rule) -> bool {
        if (ruleToObjectMap[rule] == nullptr) {
            return false;
        }
        return true;
    };

    auto checkDelimiter = [] (char c) -> bool {
        const char* delimiters = "[{.*?\0";
        for (int i = 0 ; i < strlen(delimiters) ; i++) {
            if (delimiters[i] == c) {
                return true;
            }
        }
        return false;
    };

    auto increment = [&size] (int * i) -> bool {
        if ((*i) != size - 1) {
            ++(*i);
            return true;
        } else {
            return false;
        }
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
                if (checkRuleMap(spanRule)) {
                    ruleVector.push_back(getRuleObject(spanRule));
                    break;
                }
                ruleToObjectMap[spanRule] = new Rule(spanRule, ruleVector);
            break;
            case '{':
                while (inputRegex[i] != '}') {
                    spanRule.push_back(inputRegex[i]);
                    increment(&i);
                }
                spanRule.push_back(inputRegex[i]);
                if (checkRuleMap(ruleVector[ruleVector.size()-1]->getRule()+spanRule)) {
                    ruleVector.push_back(
                        getRuleObject(ruleVector[ruleVector.size()-1]->getRule()+spanRule)
                    );
                    ruleVector.erase(ruleVector.begin()+ruleVector.size()-2);
                    break;
                }
                new CompoundRule(spanRule, ruleVector ,ruleToObjectMap);
            break;
            case '.':
                spanRule.push_back('.');
                if (checkRuleMap(spanRule)) {
                    ruleVector.push_back(getRuleObject(spanRule));
                    break;
                }
                ruleToObjectMap[spanRule] = new Rule(spanRule, ruleVector);
            break;
            case '*':
                spanRule.push_back('*');
                if (checkRuleMap(ruleVector[ruleVector.size()-1]->getRule()+spanRule)) {
                    ruleVector.push_back(
                        getRuleObject(ruleVector[ruleVector.size()-1]->getRule()+spanRule)
                    );
                    ruleVector.erase(ruleVector.begin()+ruleVector.size()-2);
                    break;
                }
                new CompoundRule(spanRule, ruleVector ,ruleToObjectMap);
            break;
            case '?':
                spanRule.push_back('?');
                if (checkRuleMap(ruleVector[ruleVector.size()-1]->getRule()+spanRule)) { 
                    ruleVector.push_back(
                        getRuleObject(ruleVector[ruleVector.size()-1]->getRule()+spanRule)
                    );
                    ruleVector.erase(ruleVector.begin()+ruleVector.size()-2);
                    break;
                }
                new CompoundRule(spanRule, ruleVector ,ruleToObjectMap);
            break;
            default:
                while (!checkDelimiter(inputRegex[i])) {
                    spanRule.push_back(inputRegex[i]);
                    if (!increment(&i)) {
                        break;
                    }
                }
                if (checkRuleMap(spanRule)) {
                    ruleVector.push_back(getRuleObject(spanRule));
                    break;
                }
                ruleToObjectMap[spanRule] = new Rule(spanRule, ruleVector);
            break;
        }
    }

    std::string reg = buildRegex(ruleVector);
    std::cout << reg << "\n\n";
    std::regex phonePattern(reg);
    std::smatch match;
    breakPoint();
    if (ruleVector.size() == 1) {
        delete ruleVector[0];
        ruleVector[0] = new Rule(inputRegex);
    } else {
        ruleVector.emplace(ruleVector.begin(), new Rule(inputRegex));
    }
    ruleToObjectMap.clear();

    while (std::regex_search(input, match, phonePattern)) {
        for (int i = 0 ; i < ruleVector.size() ;i++) {
            if (ruleToObjectMap[ruleVector[i]->getRule()] != nullptr) {
                ruleToObjectMap[ruleVector[i]->getRule()]
                ->appendToMatchVector(std::string(match[i]));
            } else {
                ruleVector[i]->setMatch(match[i]);
                ruleToObjectMap[ruleVector[i]->getRule()] = ruleVector[i];
            }
        }
        std::cout << "\n";
        printMatches(ruleVector);
        std::cout << "\n";
        ruleToObjectMap.clear();
        input = match.suffix().str();
    }

    std::sort(ruleVector.begin(), ruleVector.end());
    ruleVector.erase(std::unique(ruleVector.begin(), ruleVector.end()), ruleVector.end());
    deallocateRules(ruleVector);

    return 0;
}