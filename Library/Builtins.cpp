#include "Builtins.h"
#include <functional>
#include <algorithm>
#include <filesystem>

namespace Builtins {

    namespace Helpers {
        void recursiveList(std::vector<std::any>& retVec ,const std::filesystem::path& directory, const std::filesystem::path& basePath) {
            for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                std::filesystem::path relativePath = std::filesystem::relative(entry, basePath);
                std::string path = relativePath.string();
                retVec.push_back("./" + path);
                if (std::filesystem::is_directory(entry)) {
                    recursiveList(retVec, entry, basePath);
                }
            }
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
                    std::any_cast<std::string>(dynamic_cast<ExpressionTree*>(args[i])->getVal())
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
                std::cerr << "runtime error :: incorrect input to size function" << std::endl;
                exit(0);
            }
        }
        return std::any(0.00);
    }

    std::vector<std::any> ANYSTRINGVECTORBASE = {std::any("")};
    std::vector<std::any> ANYNUMBERVECTORBASE = {std::any(0.00)};
    std::map<std::string, FunctionType> builtInFunctions = {
        {"print", fPrint},
        {"println", fPrintln},
        {"ls", fLs},
        {"size", fSize}
    };
    std::map<std::string, std::any> baseValues = {
        {"print", std::any(0.00)},
        {"println", std::any(0.00)},
        {"ls", std::any(Builtins::ANYSTRINGVECTORBASE)},
        {"size", std::any(0.00)}
    };
}