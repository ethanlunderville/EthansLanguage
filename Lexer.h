/*

    File: Lexer.h

    Description:
    The Lexer is responsible for reading in the source code
    file and translating the assci characters in the file
    to a list of tokens. All valid tokens can be found in
    Keywords.h in the TokenType enum. Please be aware that
    the main functionality of the Lexer is implemented in 
    the scanTokens() function. All other functions in the
    class are helper functions or print funcions. 
    
*/
#pragma once
#include <iosfwd>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Keywords.h"
#include "TypeManager.h"

class TypeManager;

class Lexer {
public:
    Lexer(std::istream& inStream, TypeManager* typeManager);
    std::vector<Token> scanTokens();
    void printLexemes(const std::vector<Token>& tokens);
private:
    std::istream& inStream;
    std::vector<Token> tokens;
    TypeManager* typeManager;
    void addToken(TokenType type, int line, const std::string& lexeme);
    void string(int line);
    void number(char* c, int line, bool isNegative);
    void alphaProcessor(char* c, int line);
    void regexProcessor(char* c, int line);
    bool isEndOfString(char c);
    void regexIdentifierProcessor(char* c, int line);
    bool previousTokenTypeWas(TokenType t);
    bool previousTokenWasOperator();
};
