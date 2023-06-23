/*

    File: Lexer.h

    Description:
    The Lexer is responsible for reading in the source code
    file and translating the assci characters in the file
    to a list of tokens. All valid tokens can be found in
    Keywords.h in the TokenType enum.

    Notes:
    Please be aware that the main functionality of the 
    Lexer is implemented in the scanTokens() function.
    All other functions in the class are helper functions 
    or print funcions.
    
*/
#ifndef LEXER_H
#define LEXER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

class Lexer {
public:
    Lexer(std::istream& inStream);
    std::vector<Token> scanTokens();
    void printLexemes(std::vector<Token> tokens);
private:
    std::istream& inStream;
    std::stringstream currentLexeme;
    std::vector<Token> tokens;
    void addToken(TokenType type, int line, std::string lexeme);
    void string(std::vector<Token> tokens, int line);
    void number(char* c, std::vector<Token> tokens, int line, bool isNegative);
    void alphaProcessor(char* c, std::vector<Token> tokens, int line);
    bool previousTokenTypeWas(TokenType t);
    bool previousTokenWasOperator();
};

#endif
