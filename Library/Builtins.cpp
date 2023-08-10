#include "Builtins.h"

namespace Builtins {

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
        for (int i = 0 ; i < args.size() - 1 ; i++) {
            flags.push_back(std::any_cast<std::string>(dynamic_cast<ExpressionTree*>(args[i])->getVal()));
        }
        std::string path = "./"
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            std::cout << entry.path().filename() << std::endl;
            retVec.push_back(std::any(entry.path().filename()));
        }
        return std::any(retVec);
    }

    std::vector<std::any> ANYSTRINGVECTORBASE = {std::any("")};
    std::vector<std::any> ANYNUMBERVECTORBASE = {std::any(0.00)};

    std::map<std::string, FunctionType> builtInFunctions = {
        {"print", fPrint},
        {"println", fPrintln},
        {"ls", fLs}
    };

    std::map<std::string, std::any> baseValues = {
        {"print", std::any(0.00)},
        {"println", std::any(0.00)},
        {"ls", std::any(Builtins::ANYSTRINGVECTORBASE)}

    };
}