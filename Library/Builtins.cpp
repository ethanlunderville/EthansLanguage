#include "Builtins.h"

namespace Builtins {
    std::any fPrint(std::vector<AST*>& args, ASTChecker* checker, ASTInterpreter* interpreter){
        std::cout << "dfjvzdf;zdfjz;dfgjdskfg;dfjkglz;df" << std::endl;
        return std::any(0);
    }
    std::map<std::string, FunctionType> builtInFunctions = {
        { 
        "test", fPrint
        }
    };
}