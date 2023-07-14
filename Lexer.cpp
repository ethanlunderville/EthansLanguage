#include "Lexer.h"
#include <iostream>

Lexer::Lexer(std::istream& inStream, TypeManager* typeManager) : inStream(inStream), typeManager(typeManager) {}

std::vector<Token> Lexer::scanTokens() {
    int line = 1;
    while (inStream) {
        char c = inStream.get();
        //PLEASE TAKE NOTE OF THE SKIP LABEL
        SKIP:
        switch (c) {
            // Single-character lexemes
            case '(': addToken(LEFT_PAREN, line, "("); break;
            case ')': addToken(RIGHT_PAREN, line, ")"); break;
            case '{': addToken(LEFT_BRACE, line, "{"); break;
            case '}': addToken(RIGHT_BRACE, line, "}"); break;
            case ',': addToken(COMMA, line, ","); break;
            case '.': addToken(DOT, line, "."); break;
            case '+': addToken(PLUS, line, "+"); break;
            case ';': addToken(SEMICOLON, line, ";"); break;
            case ':': addToken(COLON, line, ":"); break;
            case '*': addToken(STAR, line, "*"); break;
            case '^': addToken(KARAT, line, "^"); break;
            case '|': addToken(OR, line, "|"); break;
            case '&': addToken(AND, line, "&"); break;
            // Two-character lexemes
            /*
            * All two character tokens skip the next token 
            * by calling inStream.get() since the token is
            * renewed with every iteration. When the loop
            * comes back and calls inStream.get() again it
            * will move to the character after the second
            * character of the two character lexemes.
            */
            case '!': 
                if (inStream.peek() == '=') { 
                    addToken(BANG_EQUAL , line, "!=");
                    c = inStream.get(); 
                } else {
                    addToken(BANG, line, "!");
                }
            break;
            case '=': 
                if (inStream.peek() == '=') { 
                    addToken(EQUAL_EQUAL  , line, "==");
                    c = inStream.get(); 
                } else {
                    addToken(EQUAL, line, "=");
                }
            break;
            case '<': 
                if (inStream.peek() == '=') { 
                    addToken(LESS_EQUAL , line, "<=");
                    c = inStream.get(); 
                } else {
                    addToken(LESS, line, "<");
                }
            break;
            case '>': 
                if (inStream.peek() == '=') { 
                    addToken(GREATER_EQUAL, line, ">=");
                    c = inStream.get(); 
                } else {
                    addToken(GREATER, line, ">");
                }
            break;
            // Negatives numbers, Comments and Slashes
            case '-': 
                if (previousTokenWasOperator()) {
                    if (isdigit(inStream.peek())) {
                        c = inStream.get();
                        number(&c,tokens, line, true);
                        goto SKIP;
                    } else {
                        std::cerr << "Error at line " << line << ": unexpected character '" << c << "'\n";
                        exit(1);
                    }
                } else {
                    addToken(MINUS, line, "-");
                } 
            break;
            case '/':
                if (inStream.peek() == '/') {
                    // A comment goes until the end of the line.
                    while (inStream && inStream.get() != '\n');
                    line++;
                } else {
                    addToken(SLASH, line, "/");
                }
                break;
            //White space
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                line++;
                break;
            // String literals
            case '"': string(tokens, line); break;
            // Numbers
            default:
                if (isdigit(c)) {
                    number(&c,tokens, line, false);
                    goto SKIP;
                } else if (isalpha(c)) {
                    alphaProcessor(&c, tokens, line);
                    goto SKIP;
                }
                break;
        }
    }
    return tokens;
}
void Lexer::printLexemes(std::vector<Token> tokens) {
    for (int i = 0 ; i < tokens.size() ; i++) {
        if (tokens[i].type == IDENTIFIER || this->typeManager->tokenIsRValue(tokens[i].type)) {
            std::cout << tokenToStringMap[tokens[i].type] << ": " << tokens[i].lexeme << std::endl;
        } else {
            std::cout << tokenToStringMap[tokens[i].type] << std::endl;        
        }
    }
}
void Lexer::addToken(TokenType type, int line, std::string lexeme) {
    struct Token token = {type, line, lexeme};
    tokens.push_back(token);
}
void Lexer::string(std::vector<Token> tokens , int line) {
    std::string literal;
    literal.push_back('\"');
    while (!inStream.eof()) {
        char temp = inStream.get();
        literal.push_back(temp);
        if (temp == '"') {
            addToken(STRING, line, literal);
            return;    
        }
    }
    std::cerr << "Error: String was not closed starting on line: " << line << std::endl;
    exit(1);
}
void Lexer::number (char *c, std::vector<Token> tokens , int line, bool isNegative) {
    std::string number; 
    while (isdigit(*c) || *c == '.') {
        number.push_back(*c);
        *c = inStream.get();
    }
    if(isNegative) {
        number.insert(0, "-");
    }
    addToken(NUMBER, line, number);
    return;
}
void Lexer::alphaProcessor (char *c, std::vector<Token> tokens , int line) {
    std::string ident; 
    while (isalpha(*c)) {
        ident.push_back(*c);
        *c = inStream.get();
    }
    if (stringToTokenMap.count(ident) > 0) {
        addToken(stringToTokenMap[ident], line, ident);
        return;    
    }
    addToken(IDENTIFIER, line, ident);
}
bool Lexer::previousTokenTypeWas(TokenType t) {
    if (this->tokens[this->tokens.size()-1].type == t) {
        return true;
    }
    return false;
}
bool Lexer::previousTokenWasOperator() {
    for (int i = 0 ; i < (sizeof(OperatorArray)/sizeof(OperatorArray[0])) ; i++) {
        if (previousTokenTypeWas(OperatorArray[i])) {
            return true;
        }
    }
    return false;
}
    



