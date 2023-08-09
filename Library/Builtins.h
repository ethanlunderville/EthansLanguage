#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"
#include <string>
#include <functional>
class ASTInterpreter;
class ASTChecker;

namespace Builtins {
    std::any fPrint(std::vector<AST*>& args, ASTChecker* checker, ASTInterpreter* interpreter);
    using FunctionType = std::function<std::any(std::vector<AST*>& args, ASTChecker* checker, ASTInterpreter* interpreter)>;
    extern std::map<std::string, FunctionType> builtInFunctions;
}


