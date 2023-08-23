#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include <string>
#include <functional>

/*BUILT IN FUNCTIONS*/

class ASTInterpreter;
class ASTChecker;

namespace Builtins {
    void setFilePath(const char* ePath);
    using FunctionType = std::function<std::any(std::vector<AST*>& args)>;
    extern std::map<std::string, FunctionType> builtInFunctions;
    extern std::map<std::string, std::any> baseValues;
}


