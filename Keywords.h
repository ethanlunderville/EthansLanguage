#include <iostream>
#include <map>
#include <string>

enum TokenType {
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL, 
    IDENTIFIER, STRING, NUMBER,
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NONE, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, 
    INT,BOOL, STRINGTYPE, RESERVED,
    EOF_TOKEN
};

static std::map<TokenType, std::string> tokenToStringMap = {
    {LEFT_PAREN, "LEFT_PAREN"},
    {RIGHT_PAREN, "RIGHT_PAREN"},
    {LEFT_BRACE, "LEFT_BRACE"},
    {RIGHT_BRACE, "RIGHT_BRACE"},
    {COMMA, "COMMA"},
    {DOT, "DOT"},
    {MINUS, "MINUS"},
    {PLUS, "PLUS"},
    {SEMICOLON, "SEMICOLON"},
    {SLASH, "SLASH"},
    {STAR, "STAR"},
    {BANG, "BANG"},
    {BANG_EQUAL, "BANG_EQUAL"},
    {EQUAL, "EQUAL"},
    {EQUAL_EQUAL, "EQUAL_EQUAL"},
    {GREATER, "GREATER"},
    {GREATER_EQUAL, "GREATER_EQUAL"},
    {LESS, "LESS"},
    {LESS_EQUAL, "LESS_EQUAL"},
    {IDENTIFIER, "IDENTIFIER"},
    {STRING, "STRING"},
    {NUMBER, "NUMBER"},
    {AND, "AND"},
    {CLASS, "CLASS"},
    {ELSE, "ELSE"},
    {FALSE, "FALSE"},
    {FUN, "FUN"},
    {FOR, "FOR"},
    {IF, "IF"},
    {NONE, "NONE"},
    {OR, "OR"},
    {PRINT, "PRINT"},
    {RETURN, "RETURN"},
    {SUPER, "SUPER"},
    {THIS, "THIS"},
    {TRUE, "TRUE"},
    {VAR, "VAR"},
    {WHILE, "WHILE"},
    {EOF_TOKEN, "EOF_TOKEN"},
    {INT, "INT"},
    {BOOL, "BOOL"},
    {STRING, "STRING"},
    {RESERVED, "RESERVED"}
};

static std::map<std::string, TokenType> stringToTokenMap = {
    {"and", AND},
    {"else", ELSE},
    {"false", FALSE},
    {"fun", FUN},
    {"for", FOR},
    {"if", IF},
    {"none", NONE},
    {"or", OR},
    {"print", PRINT},
    {"return", RETURN},
    {"true", TRUE},
    {"var", VAR},
    {"while", WHILE},
    {"EOF", EOF_TOKEN},
    //TYPES
    {"int", INT},
    {"bool", BOOL},
    {"string", STRINGTYPE}
};
