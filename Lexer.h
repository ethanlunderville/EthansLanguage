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

     +-----+    ε    +-----+    0    +-----+
 |  q0 |----->|  q1 |----->|  q2 |----->|  q3 |
 +-----+         +-----+         +-----+     |
    |                                   ^    |
    |                                   |ε   |
    |                                   |    |
    |ε                                  |    |
    v                                   |ε   |
 +-----+    1    +-----+    ε    +-----+    |
 |  q4 |----->|  q5 |----->|  q6 |----------+
 +-----+         +-----+         +-----+
    |                                   |
    |ε                                  |
    |                                   |
    v                                   |
 +-----+         +-----+         +-----+
 |  q7 |----->|  q8 |----->|  q9 |----->| q10 |
 +-----+         +-----+         +-----+

  _______    _______    _______    _______    _______    _______    _______
 / ___   \  / ___   \  / ___   \  / ___   \  / ___   \  / ___   \  / ___   \
/ /__/___/ / /__/___/ / /__/___/ / /__/___/ / /__/___/ / /__/___/ / /__/___/
\_________/\_________/\_________/\_________/\_________/\_________/\_________/
    
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
    void printLexemes(std::vector<Token> tokens);
private:
    std::istream& inStream;
    std::stringstream currentLexeme;
    std::vector<Token> tokens;
    TypeManager* typeManager;
    void addToken(TokenType type, int line, std::string lexeme);
    void string(std::vector<Token> tokens, int line);
    void number(char* c, std::vector<Token> tokens, int line, bool isNegative);
    void alphaProcessor(char* c, std::vector<Token> tokens, int line);
    bool previousTokenTypeWas(TokenType t);
    bool previousTokenWasOperator();
};
