/*
    
    This file contains several data structures pertaining 
    to the tokens of the langauge.
    
*/

#pragma once
#include <map>
#include <string>

enum Tokens {
    PLACEHOLDER, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, ARROW, MINUS, PLUS, SEMICOLON, DIVIDE, STAR,
    KARAT, EXCLAIM, EXCLAIM_EQUAL, EQUAL, EQUAL_EQUAL, GREATER,
    GREATER_EQUAL, LESS, LESS_EQUAL, IDENTIFIER, STRING,
    NUMBER, AND, ELSE, FOR, IF, NULLTYPE, OR, RETURN,
    WHILE, INT, STRINGTYPE, COLON ,EOF_TOKEN, LEFT_BRACKET,
    RIGHT_BRACKET, STRUCT, NEW, EQUALARROW, REGEX
};

static const Tokens OperatorArray[] = {   
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,
    EQUAL_EQUAL,
    EXCLAIM_EQUAL,
    KARAT,
    PLUS,
    STAR,
    MINUS,
    AND,
    OR,
    DIVIDE,
    ARROW,
    EQUAL
};

static const Tokens Statement[] = {   
    IF,
    WHILE,
    RETURN,
    FOR,
    IDENTIFIER
};


static std::map<Tokens, std::string> tokenToStringMap = {
    {LEFT_PAREN, "LEFT_PAREN"},
    {RIGHT_PAREN, "RIGHT_PAREN"},
    {LEFT_BRACE, "LEFT_BRACE"},
    {RIGHT_BRACE, "RIGHT_BRACE"},
    {COMMA, "COMMA"},
    {ARROW, "ARROW"},
    {MINUS, "MINUS"},
    {PLUS, "PLUS"},
    {SEMICOLON, "SEMICOLON"},
    {COLON, "COLON"},
    {DIVIDE, "DIVIDE"},
    {STAR, "STAR"},
    {EXCLAIM, "EXCLAIM"},
    {EXCLAIM_EQUAL, "EXCLAIM_EQUAL"},
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
    {ELSE, "ELSE"},
    {IF, "IF"},
    {NULLTYPE, "NULLTYPE"},
    {OR, "OR"},
    {RETURN, "RETURN"},
    {WHILE, "WHILE"},
    {EOF_TOKEN, "EOF_TOKEN"},
    {INT, "INT"},
    {STRING, "STRING"},
    {STRINGTYPE, "STRINGTYPE"},
    {KARAT, "KARAT"},
    {LEFT_BRACKET,"LEFT_BRACKET"},
    {RIGHT_BRACKET,"RIGHT_BRACKET"},
    {STRUCT,"STRUCT"},
    {NEW, "NEW"},
    {FOR, "FOR"},
    {EQUALARROW, "EQUALARROW"},
    {REGEX, "REGEX"}
};

static std::map<std::string, Tokens> stringToTokenMap = {
    {"else", ELSE},
    {"if", IF},
    {"NULL", NULLTYPE},
    {"return", RETURN},
    {"while", WHILE},
    {"for", FOR},
    {"EOF", EOF_TOKEN},
    {"int", INT},
    {"string", STRINGTYPE},
    {"struct", STRUCT},
    {"new", NEW}
};

struct Token {
    Tokens type;
    int line;
    std::string lexeme;
};
