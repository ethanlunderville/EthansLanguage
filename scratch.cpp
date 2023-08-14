#include <iostream>
#include <any>
#include <vector>
#include <string>
std::any fReplace(std::vector<AST*>& args) {
    std::string retString;
    std::string mainStr = args[0];
    std::string toBeReplaced = args[1];
    std::string replacer = args[2];
    std::vector<int> indexes;
    for (int i = 0 ; i < mainStr.size() ; i++) {
        if (Helpers::match(mainStr, toBeReplaced,i)) {
            indexes.push_back(i);
            indexes.push_back(i + toBeReplaced.size());
        }
    }
    for (int i = 0 ; i < indexes.size() ; i++) {
        
    }
}

int main () {
    std::vector<std::string> args = {std::string("r0cgf r0 this r0is a r0 testr0"),std::string("")};
    std::vector<std::any> results = std::any_cast<std::vector<std::any>>(fSplit(args));
    for (int i = 0 ; i < results.size(); i++) {
        std::cout << std::any_cast<std::string>(results[i]) << std::endl;
    }

    return 0;
}