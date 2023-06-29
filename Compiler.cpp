/*

    File: Compiler.cpp

    Description:
    
    The main driver code of the compiler. Strings 
    together all of the main Compiler components.

    COMPONENTS: 

    *** input -> component -> output ***

    Sourcefile.el -> LEXER -> ProgramTokens 
    ProgramTokens -> PARSER -> AbstractSyntaxTree
    
*/

//#include "Keywords.h"
//#include "Lexer.cpp" // NO NEED TO OVERCOMPLICATE
//#include "Parser.cpp"
#include "Table.h"
//#include "Visitors/ASTVisitor.h"

int main () {
    /*
    std::ifstream file("./test.c");
    Lexer* lexer = new Lexer(file);
    Parser* parser = new Parser(lexer);
    std::cout << "***PRINTING LEXEMES***" << std::endl; 
    lexer->printLexemes(lexer->scanTokens());
    ASTPrintVisitor* pVisit = new ASTPrintVisitor();
    std::cout << "***PRINTING AST***" << std::endl;
    AST* abstractSyntaxTree = parser->parse();
    abstractSyntaxTree->accept(pVisit);
    std::cout << "***DEALLOCATING AST***" << std::endl;
    ASTDeallocationVisitor* dVisit = new ASTDeallocationVisitor();
    abstractSyntaxTree->accept(dVisit);
    */

    SymbolTable* baseTable = new SymbolTable(nullptr);
    SymbolTable* otherTable = new SymbolTable(baseTable);
    //void declareSymbol(int line, std::string identifier , std::any value, std::string type)
    std::any name = std::string("1");
    baseTable->declareSymbol(1 , "x", name, "string");
    baseTable->declareSymbol(1 , "y", 1, "int");
    //baseTable->declareSymbol(2 , "y", 1, "int");
    //otherTable->declareSymbol(4, "goober", 1, "int");
    //otherTable->declareSymbol(4, "tee", 1, "int");
    //otherTable->pushScope();
    //otherTable->declareSymbol(4, "teener", 1, "int");
    //otherTable->declareSymbol(4, "t", 1, "int");
    baseTable->printSymbolTable();
    std::any x = std::string("string literal");
    baseTable->reassignSymbol("x", x);
    baseTable->reassignSymbol("y", 33);
    int a = std::any_cast<int>(baseTable->getValueStoredInSymbol("y"));
    std::string y = std::any_cast<std::string>(baseTable->getValueStoredInSymbol("x"));
    baseTable->printSymbolTable();
    std::cout << y << std::endl;
    std::cout << a << std::endl;
    /*
    delete pVisit;
    pVisit = nullptr;
    delete parser;
    parser = nullptr;
    delete lexer;
    lexer = nullptr;
    delete dVisit;
    dVisit = nullptr;
    return 0;
    */
}