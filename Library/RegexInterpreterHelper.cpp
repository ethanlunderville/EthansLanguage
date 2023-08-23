#include "Regex.h"
#include "SymbolTable/ContextManager.h"
#include "Visitors/ASTVisitor.h"
#include "SyntaxTree/AST.h"

static void printVec(std::vector<Rule*>& ruleVector) {
    for (Rule* rule : ruleVector) {
        rule->printer();
        std::cout << "\n";
    }
}

static void deallocateRules(std::vector<Rule*>& ruleVector) {
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

static std::string buildRegex(std::vector<Rule*>& ruleVector) {
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

static void printMatches(std::vector<Rule*>& ruleVector) {
    for (int i = 0 ; i < ruleVector.size() ; i++) {
        ruleVector[i]->printMatch();
        std::cout << "\n";
    }
}

static void breakPoint() {return;}

void RegexInterpreterManager::runRegexSection (
    std::string inputRegex, std::string input, 
    ASTInterpreter* interpreter, AST* regexSectionTree
) {
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
    bool finished = false;
    int start = 0;
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
                start = i;
                while (inputRegex[i] != '}') {
                    spanRule.push_back(inputRegex[i]);
                    increment(&i);
                    if ( 
                    (i - start == 4 || (i - start == 1 || i - start == 3) && !isdigit(inputRegex[i])) 
                    || 
                    (i - start == 2 && inputRegex[i] != ',' && inputRegex[i] != '}')
                    ) {
                        std::cerr 
                        << "Error:: bad regex, regexes may only contain one whole number or two whole numbers seperated by a comma"
                        << std::endl;
                        exit(1);
                    }
                }
                spanRule.push_back(inputRegex[i]);
                if (ruleVector.size() == 0) {
                    std::cerr << "{} rule may not be the first rule in a regex." << std::endl;
                    exit(1);
                }
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
            case '+':
            case '*':
                spanRule.push_back('+');
                if (ruleVector.size() == 0) {
                    std::cerr << "+ and * rules may not be the first rule in a regex." << std::endl;
                    exit(1);
                }
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
                if (ruleVector.size() == 0) {
                    std::cerr << "? rule may not be the first rule in a regex." << std::endl;
                    exit(1);
                }
                if (checkRuleMap(ruleVector[ruleVector.size()-1]->getRule()+spanRule)) { 
                    ruleVector.push_back(
                        getRuleObject(ruleVector[ruleVector.size()-1]->getRule()+spanRule)
                    );
                    ruleVector.erase(ruleVector.begin()+ruleVector.size()-2);
                    break;
                }
                new CompoundRule(spanRule, ruleVector ,ruleToObjectMap);
            break;
            case '\\':
                if (!increment(&i)) {
                    std::cerr 
                    << "escape cannot be the last character in the sequence"
                    << std::endl;
                    exit(1);
                }
                spanRule.push_back('\\');
                spanRule.push_back(inputRegex[i]);
                ruleToObjectMap[spanRule] = new Rule(spanRule, ruleVector);
            break;
            default: 
                while (!checkDelimiter(inputRegex[i])) {
                    spanRule.push_back(inputRegex[i]);
                    if (!increment(&i)) {
                        finished = true;
                        break; 
                    }
                }
                --i;
                if (checkRuleMap(spanRule)) {
                    ruleVector.push_back(getRuleObject(spanRule));
                    if (finished) 
                        goto END;
                    break;
                }
                ruleToObjectMap[spanRule] = new Rule(spanRule, ruleVector);
                if (finished) 
                    goto END;
            break;
        }
    }
    END:
    std::string reg = buildRegex(ruleVector);
    #ifdef REGEXDEBUG
        std::cout << reg << "\n\n";
    #endif
    std::regex phonePattern(reg, std::regex_constants::awk);
    std::smatch match;
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
        #ifdef REGEXDEBUG
            std::cout << "\n";
            printMatches(ruleVector);
        #endif
        std::vector<std::string> memorizer;
        interpreter->contextManager->pushScope();
        for (int i = 0 ; i < ruleVector.size() ; i++) {
            breakPoint();
            std::string rule = "$"+ruleVector[i]->getRule()+"$";
            auto it = std::find(memorizer.begin(), memorizer.end(), rule);
            if (it == memorizer.end() ) {
                interpreter->contextManager->declareSymbol(
                    0 , rule , interpreter->typeManager->getTypeHandler("string")
                );
                std::any* address = interpreter->contextManager->getReferenceOfValueStoredInSymbol(rule);
                (*address) = ruleVector[i]->matched;
                memorizer.push_back(rule);
            }
        }
        interpreter->visitChildren(regexSectionTree);
        interpreter->contextManager->popScope(false);
        #ifdef REGEXDEBUG
            std::cout << "\n";
        #endif
        ruleToObjectMap.clear();
        input = match.suffix().str();
    }    
    std::sort(ruleVector.begin(), ruleVector.end());
    ruleVector.erase(
        std::unique(ruleVector.begin(), ruleVector.end()), 
        ruleVector.end()
    );
    deallocateRules(ruleVector);
}

/*
int main () {
    // },] does not have to be escaped
    // {,[ does have to be escaped    
    std::string inputRegex = "[0-9]{3}-[0-9]{3}-[0-9]{4}";
    std::ifstream inputFile("./test");
    if (!inputFile) {
        std::cerr << "Failed to open the file." << std::endl;
    }
    std::string input(
        (std::istreambuf_iterator<char>(inputFile)), 
        (std::istreambuf_iterator<char>())
    );
    RegexInterpreterManager* rManage = new RegexInterpreterManager();
    rManage->runRegexSection(inputRegex, input);
    delete rManage;
    return 0;
}*/