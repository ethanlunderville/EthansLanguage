/*

    File: Compiler.cpp

    Description:
    
    The main driver code of the compiler. Strings 
    together all of the main Compiler components.

    COMPONENTS: 

    *** input -> component -> output ***

    Sourcefile.el -> LEXER -> ProgramTokens 
    ProgramTokens -> PARSER -> AbstractSyntaxTree
    AbstractSyntaxTree -> CHECKER -> CheckerAbstractSyntaxTree
    if [TREEWALKER] 
        CheckedAbstractSyntaxTree -> TREEWALKER -> ProgramOutput
    else if [LLVM backend] -> NOT IMPLEMENTED YET
        CheckedAbstractSyntaxTree -> LLVM Backend -> ExecutableFile

    
*/
//            declString -> tokenType
// string ->  tokenType -> ASTNode -> TypeHandler

#include "Lexer.h"
#include "Parser.h"
#include "SymbolTable/ContextManager.h"
#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

/*


                            .__.__                
  ____  ____   _____ ______ |__|  |   ___________ 
_/ ___\/  _ \ /     \\____ \|  |  | _/ __ \_  __ \
\  \__(  <_> )  Y Y  \  |_> >  |  |_\  ___/|  | \/
 \___  >____/|__|_|  /   __/|__|____/\___  >__|   .cpp
     \/            \/|__|                \/       



*/

TypeManager* initPrimitiveTypes(){
    TypeManager* tManage = new TypeManager();
    tManage->addTypeDecl(std::string("string"), STRINGTYPE, String::getInstance());
    tManage->addTypeRVal(STRING, String::getInstance());
    tManage->addTypeDecl(std::string("int"), INT, Number::getInstance());
    tManage->addTypeRVal( NUMBER, Number::getInstance());
    return tManage;
}

int main () {
    TypeManager* typeManager = initPrimitiveTypes();
    ASTPrintVisitor* pVisit = new ASTPrintVisitor();
    std::ifstream file("./test.c");
    Lexer* lexer = new Lexer(file, typeManager);
    Parser* parser = new Parser(lexer, typeManager);
    std::cout << "***PRINTING LEXEMES***" << std::endl; 
    lexer->printLexemes(lexer->scanTokens());
    AST* abstractSyntaxTree = parser->parse();
    ASTChecker* checker = new ASTChecker(typeManager);
    std::cout << "***PRINTING AST***" << std::endl;
    abstractSyntaxTree->accept(pVisit);
    std::cout << "***CHECKING AST***" << std::endl;
    abstractSyntaxTree->accept(checker);
    //std::cout << "***PRINTING AST***" << std::endl;
    //abstractSyntaxTree->accept(pVisit);
    //std::cout << "***INTERPRETING AST***" << std::endl;
    //ASTInterpreter* interpreter = new ASTInterpreter(typeManager);
    //LET THE FUN BEGIN
    //abstractSyntaxTree->accept(interpreter);
    //std::cout << "***DEALLOCATING AST***" << std::endl;
    ASTDeallocationVisitor* dVisit = new ASTDeallocationVisitor();
    //abstractSyntaxTree->accept(dVisit);
    delete typeManager;
    typeManager = nullptr;
    delete pVisit;
    pVisit = nullptr;
    delete parser;
    parser = nullptr;
    delete lexer;
    lexer = nullptr;
    delete dVisit;
    dVisit = nullptr;
    
    /*
    ContextManager(TypeManager* typeManager);
    ~ContextManager();
    void pushContext();
    void popContext();
    void pushScope();
    void popScope();
    void declareSymbol(int line, std::string identifier, std::string type);
    void reassignSymbol(std::string identifier, std::any value, int line);
    void contextPopRecurse(SymbolTable* sym);
    void printSymbolTable();
    std::any getValueStoredInSymbol(std::string identifier);
    Type* getTypeOfSymbol(std::string identifier);
    */
    /*
    ContextManager* cm = new ContextManager(typeManager);
    cm->declareSymbol(1, std::string("hello"), std::string("int"));
    std::cout << "\n\n------------------\n\n";
    cm->printSymbolTable();
    cm->reassignSymbol(std::string("hello"), 11.00 , 2);
    std::cout << "\n\n------------------\n\n";
    cm->printSymbolTable();
    cm->pushScope();
    cm->declareSymbol(1, std::string("g"), std::string("int"));
    cm->reassignSymbol(std::string("g"), 2222.00 , 2);
    cm->declareSymbol(1, std::string("r"), std::string("int"));
    cm->reassignSymbol(std::string("r"), 2232.00 , 2);
    std::cout << "\n\n------------------\n\n";
    cm->printSymbolTable();

    cm->pushContext();
    cm->declareSymbol(1, std::string("g"), std::string("int"));
    cm->reassignSymbol(std::string("g"), 2222.00 , 2);
    std::cout << "\n\n------------------\n\n";
    cm->printSymbolTable();
    cm->popContext();
    std::cout << "\n\n------------------\n\n";
    cm->printSymbolTable();

    cm->popScope();
    std::cout << "\n\n------------------\n\n";
    cm->printSymbolTable();

    delete cm;
    */
    return 0;
}