/*

    File: Keywords.h

    Description:
    
    This file contains several data structures pertaining 
    to the tokens of the langauge.
    
*/
#pragma once
#include <map>
#include <string>

enum TokenType {
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, ARROW, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    KARAT, BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER,
    GREATER_EQUAL, LESS, LESS_EQUAL, IDENTIFIER, STRING,
    NUMBER, AND, ELSE, FOR, IF, NULLTYPE, OR, PRINT, RETURN,
    WHILE, INT, STRINGTYPE, COLON ,EOF_TOKEN, LEFT_BRACKET,
    RIGHT_BRACKET, STRUCT, NEW, EQUALARROW
};

static const TokenType OperatorArray[] = {   
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,
    EQUAL_EQUAL,
    BANG_EQUAL,
    KARAT,
    PLUS,
    STAR,
    MINUS,
    AND,
    OR,
    SLASH,
    ARROW,
    EQUAL
};

static const TokenType Statement[] = {   
    IF,
    WHILE,
    RETURN,
    IDENTIFIER
};


static std::map<TokenType, std::string> tokenToStringMap = {
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
    {ELSE, "ELSE"},
    {IF, "IF"},
    {NULLTYPE, "NULLTYPE"},
    {OR, "OR"},
    {PRINT, "PRINT"},
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
    {EQUALARROW, "EQUALARROW"}
};

static std::map<std::string, TokenType> stringToTokenMap = {
    {"else", ELSE},
    {"if", IF},
    {"NULL", NULLTYPE},
    {"return", RETURN},
    {"while", WHILE},
    {"EOF", EOF_TOKEN},
    {"int", INT},
    {"string", STRINGTYPE},
    {"struct", STRUCT},
    {"new", NEW}
};

struct Token {
    TokenType type;
    int line;
    std::string lexeme;
};
