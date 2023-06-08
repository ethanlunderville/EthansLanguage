#include "Keywords.h"
#include "Lexer.cpp"
#include "Parser.cpp"

int main (int argc, char * argv []) {

    std::ifstream file("./test");

    Lexer* lexer = new Lexer(file);
    Parser* parser = new Parser(lexer); 
    lexer->printLexemes(lexer->scanTokens());

    delete parser;
    parser = nullptr;

    delete lexer;
    lexer = nullptr;
    
    return 0;

}