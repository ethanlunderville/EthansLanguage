/*

    File: Compiler.cpp

    Description:
    
    The main driver code of the compiler. Strings 
    together all of the main Compiler components.

    COMPONENTS: 

    *** input -> component -> output ***

    Sourcefile.el -> LEXER -> ProgramTokens 
    ProgramTokens -> PARSER -> AbstractSyntaxTree
    if [TREEWALKER] 
        AbstractSyntaxTree -> TREEWALKER -> ProgramOutput

    
*/
//            declString -> tokenType
// string ->  tokenType -> ASTNode -> TypeHandler

//#include "Lexer.h"
//#include "Parser.h"
#include "SymbolTable/ContextManager.h"
#include "SyntaxTree/AST.h"
//#include "Visitors/ASTVisitor.h"

TypeManager* initTypes(){
    TypeManager* tManage = new TypeManager();
    tManage->addTypeDecl(std::string("string"), STRINGTYPE, String::getInstance());
    tManage->addTypeRVal(STRING, String::getInstance());
    tManage->addTypeDecl(std::string("int"), INT, Number::getInstance());
    tManage->addTypeRVal( NUMBER, Number::getInstance());
    return tManage;
}

int main () {
    TypeManager* typeManager = initTypes();
    /*
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
    abstractSyntaxTree->accept(checker);
    std::cout << "***PRINTING AST***" << std::endl;
    abstractSyntaxTree->accept(pVisit);
    std::cout << "***INTERPRETING AST***" << std::endl;
    ASTInterpreter* interpreter = new ASTInterpreter(typeManager);
    //LET THE FUN BEGIN
    abstractSyntaxTree->accept(interpreter);
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
    */
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
    ContextManager* cm = new ContextManager(typeManager);
    cm->declareSymbol(1, std::string("hello"), std::string("int"));
    std::cout << "------------------\n";
    cm->printSymbolTable();
    cm->reassignSymbol(std::string("hello"), 11.00 , 2);
    std::cout << "------------------\n";
    cm->printSymbolTable();
    cm->pushScope();
    cm->declareSymbol(1, std::string("g"), std::string("int"));
    cm->reassignSymbol(std::string("g"), 2222.00 , 2);
    cm->declareSymbol(1, std::string("r"), std::string("int"));
    cm->reassignSymbol(std::string("r"), 2232.00 , 2);
    std::cout << "------------------\n";
    cm->printSymbolTable();
    cm->pushScope();
    cm->declareSymbol(1, std::string("b"), std::string("int"));
    cm->reassignSymbol(std::string("b"), 262.00 , 2);
    std::cout << "------------------\n";
    cm->printSymbolTable();
    cm->pushContext();
    std::cout << "------------------\n";
    cm->printSymbolTable();
    cm->declareSymbol(1, std::string("n"), std::string("int"));
    cm->reassignSymbol(std::string("n"), 22333332.00 , 2);
    std::cout << "------------------\n";
    cm->printSymbolTable();
    cm->popContext();
    std::cout << "------------------\n";
    cm->printSymbolTable();
    cm->reassignSymbol(std::string("b"), 244.00 , 2);
    cm->popScope();
    std::cout << "------------------\n";
    cm->printSymbolTable();
    cm->reassignSymbol(std::string("hello"), "11.00" , 2);

    delete cm;
    return 0;
}