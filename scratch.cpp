#include <iostream>
#include <any>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <filesystem>
#include <fstream>
namespace Helpers {

        void recursiveList(std::vector<std::any>& retVec,
        const std::filesystem::path& directory, 
        const std::filesystem::path& basePath
        ) {
            for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                std::filesystem::path relativePath = std::filesystem::relative(entry, basePath);
                std::string path = relativePath.string();
                retVec.push_back("./" + path);
                if (std::filesystem::is_directory(entry)) {
                    recursiveList(retVec, entry, basePath);
                }
            }
        }

        int match(std::string str, std::string delim, int i) {
            int j = 0;
            int record = i;
            while (j < delim.size()) {
                if (i == str.size() - 1 && j != delim.size() -1) {
                    return false;
                } else if (str[i] != delim[j]) {
                    return false;
                }
                j++;
                i++;
            }
            return true;
        }

    }
std::any fReplace(std::vector<std::string>& args) {
    std::string retString;
    std::string mainStr = args[0];
    std::string toBeReplaced = args[1];
    std::string replacer = args[2];
    std::vector<int> indexes = {0};
    for (int i = 0 ; i < mainStr.size() ; i++) {
        if (Helpers::match(mainStr, toBeReplaced,i)) {
            indexes.push_back(i);
            indexes.push_back(i + toBeReplaced.size());
        }
    }
    indexes.push_back(mainStr.size());
    if (indexes.size() == 2) {
        return std::any(mainStr);
    } else if (indexes.size() % 2 != 0) {
        std::cerr 
        << "problem occured in replace function" 
        << std::endl;
    }
    std::vector<std::string> stringVec;
    for (int i = 1 ; i < indexes.size() ; i = i + 2) {
        stringVec.push_back(
            std::move(mainStr.substr(
                indexes[i-1], 
                indexes[i] - indexes[i-1]
            ))
        );
    } 
    int stringVecSize = stringVec.size();
    for (int i = 0 ; i < stringVecSize ; i++) {
        retString = retString + stringVec[i];
        if (i != stringVecSize - 1) {
            retString = retString + replacer;
        }
    }
    return std::any(retString);
}

int main () {
    std::vector<std::string> args = 
    {std::string("this is a test"),std::string(" "), std::string("")};
    std::string results = std::any_cast<std::string>(fReplace(args));
    std::cout << results << std::endl;
    return 0;
}