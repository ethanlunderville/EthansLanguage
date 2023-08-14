#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include <string>
#include <functional>

/*BUILT IN FUNCTIONS*/

class ASTInterpreter;
class ASTChecker;

namespace Builtins {
    std::any fPrint(std::vector<AST*>& args);
    std::any fPrintln(std::vector<AST*>& args);
    std::any fLs(std::vector<AST*>& args);
    std::any fSize(std::vector<AST*>& args);
    using FunctionType = std::function<std::any(std::vector<AST*>& args)>;
    extern std::map<std::string, FunctionType> builtInFunctions;
    extern std::map<std::string, std::any> baseValues;
}


