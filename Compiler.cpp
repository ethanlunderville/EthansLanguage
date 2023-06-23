/*

    File: Compiler.cpp

    Description:
    
    The main driver code of the compiler. Strings 
    together all of the main Compiler components.

    COMPONENTS: 

    *** input -> component -> output ***

    Sourcefile.EL -> LEXER -> ProgramTokens 
    ProgramTokens -> PARSER -> AbstractSyntaxTree
    
*/

#include "Keywords.h"
#include "Lexer.cpp" // NO NEED TO OVERCOMPLICATE
#include "Parser.cpp"
#include "Table.cpp"
#include "Visitors/ASTVisitor.h"

int main () {
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
    delete pVisit;
    pVisit = nullptr;
    delete parser;
    parser = nullptr;
    delete lexer;
    lexer = nullptr;
    delete dVisit;
    dVisit = nullptr;
    return 0;
}