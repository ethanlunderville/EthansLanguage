#include "Builtins.h"
#include <functional>
#include <algorithm>
#include <filesystem>
#include <fstream>

namespace Builtins {

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

    std::any fPrint(std::vector<AST*>& args){
        if (args.size() != 1) {return std::any(0.00);}
        std::any val = dynamic_cast<ExpressionTree*>(args[0])->getVal(); 
        if (val.type() == typeid(std::string)) {
            std::cout << std::any_cast<std::string>(val);
        } else if (val.type() == typeid(double)) {
            std::cout << std::any_cast<double>(val);
        } else if (val.type() == typeid(std::vector<std::any>)) {
            std::vector<std::any> anyVec = std::any_cast<std::vector<std::any>>(val);
            if (anyVec.size() == 0) {
                std::cout << "[]";
            } else {
                int vecSize = anyVec.size();
                std::cout << "[";
                for (int i = 0 ; i < vecSize ; i++) {
                    if (anyVec[0].type() == typeid(std::string)) {
                        std::cout << "\"";
                        std::cout << std::any_cast<std::string>(anyVec[i]);
                        std::cout << "\"";
                    } else if (anyVec[0].type() == typeid(double)) {
                        std::cout << std::any_cast<double>(anyVec[i]);
                    }
                    if (i != vecSize - 1) {
                        std::cout << ",";
                    }
                }
                std::cout << "]";
            } 
        } else if (val.type() == typeid(FunctionCallTree*)) {
            FunctionCallTree* fCallTree = std::any_cast<FunctionCallTree*>(val);
            if (builtInFunctions[fCallTree->getIdentifier()]) {
                std::cout << "Builtin function : " << fCallTree->getIdentifier();
            } else {
                std::cout << "function : " << fCallTree->getIdentifier();
            }
        }
        return std::any(0);
    }

    std::any fPrintln(std::vector<AST*>& args){ 
        builtInFunctions["print"](args);
        std::cout << "" << std::endl;
        return std::any(0);
    }

    std::any fLs(std::vector<AST*>& args){
        std::vector<std::string> flags;
        std::vector<std::any> retVec; 
        if (args.size() != 0) {
            for (int i = 0 ; i < args.size() - 1 ; i++) {
                flags.push_back(
                    std::any_cast<std::string>(
                        dynamic_cast<ExpressionTree*>(args[i])->getVal()
                    )
                );
            }
        }
        std::string path = "./";
        if (std::find(flags.begin(), flags.end(), std::string("r")) != flags.end()) {
            Helpers::recursiveList(retVec,path,std::filesystem::absolute(path));
        } else {
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                retVec.push_back(path+entry.path().filename().string());
            }
        }
        return std::any(retVec);
    }

    std::any fSize(std::vector<AST*>& args) {
        if (args.size() != 2) {
            std::any arg = dynamic_cast<ExpressionTree*>(args[0])->getVal();
            if (arg.type() ==typeid(std::string)) {
                return std::any(static_cast<double>(std::any_cast<std::string>(arg).size()));
            } else if (arg.type() == typeid(std::vector<std::any>)) {
                return std::any(static_cast<double>(std::any_cast<std::vector<std::any>>(arg).size()));
            } else if (arg.type() == typeid(double)) {
                return arg;
            } else {
                std::cerr 
                << "runtime error :: incorrect input to size function" 
                << std::endl;
                exit(0);
            }
        }
        return std::any(0.00);
    }

    std::any fSplit(std::vector<AST*>& args) {
        std::vector<std::any> retVec;
        std::string arg = std::any_cast<std::string>(
            dynamic_cast<ExpressionTree*>(args[0])->getVal()
        );
        std::string delimiter = std::any_cast<std::string>(
            dynamic_cast<ExpressionTree*>(args[1])->getVal()
        );
        int i = 0;
        int size = arg.size();
        int lastIndex = 0;
        while (i < size) {          
            if (i == arg.size() - 1) {
                std::string sub = arg.substr(lastIndex, i - lastIndex + 1);
                if (sub.size()) {
                    retVec.push_back(std::any(sub));
                    lastIndex = i; 
                }
            } else if (Helpers::match(arg,delimiter,i)) {
                std::string sub = arg.substr(lastIndex, i - lastIndex);
                if (sub.size()) {
                    retVec.push_back(std::any(sub));
                    lastIndex = i;
                }
            }
            i++;
        }
        return std::any(retVec);
    }

    std::any fReplace(std::vector<AST*>& args) {
        std::string retString;
        std::string mainStr = std::any_cast<std::string>(args[0]);
        std::string toBeReplaced = std::any_cast<std::string>(args[1]);
        std::string replacer = std::any_cast<std::string>(args[2]);
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

    std::any fRead(std::vector<AST*>& args) {
        std::string file = std::any_cast<std::string>(
            dynamic_cast<ExpressionTree*>(args[0])->getVal()
        );
        std::ifstream inputFile(file);
        if (!inputFile) {
            std::cerr << "Failed to open the file." << std::endl;
            return 1;
        }
        std::string fileContent(
            (std::istreambuf_iterator<char>(inputFile)), 
            (std::istreambuf_iterator<char>())
        );
        inputFile.close();
        return std::any(fileContent);
    }
    std::any fAppend(std::vector<AST*>& args) {
        std::ofstream file(std::any_cast<std::string>(
            dynamic_cast<ExpressionTree*>(args[0])->getVal()
        ), std::ios::app);
        if (!file) {
            std::cerr << 
            "Failed to create/open the file." 
            << std::endl;
            return 1;
        }
        std::string strToAppend = std::any_cast<std::string>(
            dynamic_cast<ExpressionTree*>(args[1])->getVal()
        ) + '\n';
        file << strToAppend;
        file.close();
        return (0);
    }
    std::any fWrite(std::vector<AST*>& args) {
        std::ofstream file(std::any_cast<std::string>(
            dynamic_cast<ExpressionTree*>(args[0])->getVal()
        ));
        if (!file) {
            std::cerr 
            << "Failed to create/open the file." 
            << std::endl;
            return 1;
        }
        std::string strToAppend = std::any_cast<std::string>(
            dynamic_cast<ExpressionTree*>(args[1])->getVal()
        ) + '\n';
        file << strToAppend;
        file.close();
        return (0);
    }

    std::vector<std::any> ANYSTRINGVECTORBASE = {std::any("")};
    std::vector<std::any> ANYNUMBERVECTORBASE = {std::any(0.00)};
    std::map<std::string, FunctionType> builtInFunctions = {
        {"print", fPrint},
        {"println", fPrintln},
        {"ls", fLs},
        {"size", fSize},
        {"split", fSplit},
        {"replace", fReplace},
        {"read", fRead},
        {"append", fAppend},
        {"write", fWrite}
    };
    std::map<std::string, std::any> baseValues = {
        {"print", std::any(0.00)},
        {"println", std::any(0.00)},
        {"ls", std::any(Builtins::ANYSTRINGVECTORBASE)},
        {"size", std::any(0.00)},
        {"size", std::any(0.00)},
        {"split", std::any(Builtins::ANYSTRINGVECTORBASE)},
        {"replace", std::any(std::string(""))},
        {"read", std::any(std::string(""))},
        {"append", std::any(0)},
        {"write", std::any(0)}                
    };
}