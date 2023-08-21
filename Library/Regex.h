#include <iostream>
#include <any>
#include <vector>
#include <map>
#include <string>
#include <regex>
#include <algorithm>
#include <functional>
#include <any>
#include <fstream>

class AST;
class ASTInterpreter;
class ASTChecker;
class ContextManager;

struct Rule {
    Rule();
    virtual ~Rule();
    Rule(std::string& rule);
    Rule(std::string& rule, std::vector<Rule*>& ruleVec);
    virtual void printer();
    virtual std::string getRule();
    void printMatch();
    void appendToMatchVector(std::string match);
    void setMatch(std::string match);

    std::string ruleString;
    std::any matched;
    bool isChild;
};

struct CompoundRule : public Rule {
    CompoundRule(std::string& rule, std::vector<Rule*>& ruleVec, std::map<std::string, Rule*>& ruleToObjectMap);
    ~CompoundRule();
    void printer() override;
    std::string getRule() override;
    Rule* childRule;
};

struct RegexInterpreterManager {
    void runRegexSection (
        std::string inputRegex, 
        std::string input, 
        ASTInterpreter* interpreter, 
        AST* regexSectionTree
    );
};