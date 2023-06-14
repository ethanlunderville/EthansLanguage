
#include "Keywords.h"
#include "Lexer.cpp" // NO NEED TO OVERCOMPLICATE
#include "Parser.cpp"
#include "Visitors/ASTVisitor.h"

int main (int argc, char * argv []) {

    std::ifstream file("./test.c");

    Lexer* lexer = new Lexer(file);
    Parser* parser = new Parser(lexer); 
    lexer->printLexemes(lexer->scanTokens());
    ASTPrintVisitor* pVisit = new ASTPrintVisitor();

    std::cout << "***PRINTING AST*** \n\n";
    parser->parse()->accept(pVisit);

    delete pVisit;
    pVisit = nullptr;
    delete parser;
    parser = nullptr;
    delete lexer;
    lexer = nullptr;
    return 0;

}