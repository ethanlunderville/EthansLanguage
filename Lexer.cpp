#include "Lexer.h"
#include <iostream>

Lexer::Lexer(std::istream& inStream, TypeManager* typeManager) 
: inStream(inStream), 
  tokens({}),
  typeManager(typeManager)
  {}

std::vector<Token> Lexer::scanTokens() {
    int line = 1;
    while (inStream) {
        char c = inStream.get();
        SKIP:
        switch (c) {
            case '(': addToken(LEFT_PAREN, line, "("); break;
            case ')': addToken(RIGHT_PAREN, line, ")"); break;
            case '[': addToken(LEFT_BRACKET, line, "["); break;
            case ']': addToken(RIGHT_BRACKET, line, "]"); break;
            case '{': addToken(LEFT_BRACE, line, "{"); break;
            case '}': addToken(RIGHT_BRACE, line, "}"); break;
            case ',': addToken(COMMA, line, ","); break;
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
                    addToken(EXCLAIM_EQUAL , line, "!="); 
                    c = inStream.get();
                } else {
                    addToken(EXCLAIM, line, "!");
                }
            break;
            case '=': 
                if (inStream.peek() == '=') { 
                    addToken(EQUAL_EQUAL  , line, "==");
                    c = inStream.get(); 
                } else if (inStream.peek() == '>') {
                    addToken(EQUALARROW , line, "=>");
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
            // Negatives numbers, Comments and DIVIDEes
            case '-': 
                if (tokens[tokens.size() - 1].type == IDENTIFIER && inStream.peek() == '-') { // ++ operator
                    addToken(EQUAL, line, "=");
                    addToken(IDENTIFIER, line, tokens[tokens.size() - 2].lexeme);
                    addToken(MINUS, line, "-");
                    addToken(NUMBER, line, "1.0");
                    c = inStream.get();
                    c = inStream.get();
                    goto SKIP;                                       
                } else if (isdigit(inStream.peek())) {
                    c = inStream.get();
                    number(&c, line, true);
                    goto SKIP;
                } else if (inStream.peek() == '>'){
                    addToken(ARROW , line, "->");
                    c = inStream.get(); 
                } else {
                    addToken(MINUS, line, "-");
                } 
            break;
            case '/':
                if (inStream.peek() == '*') {
                    while(!inStream.eof()) {
                        if (c == '\n') {
                            line++;
                        } else if (c == '*' && inStream.peek() == '/') {
                            c = inStream.get();
                            break;
                        }
                        c = inStream.get();
                    }
                } else if (inStream.peek() == '/') {
                    // A comment goes until the end of the line.
                    while (inStream && inStream.get() != '\n');
                    line++;
                } else {
                    addToken(DIVIDE, line, "/");
                }
                break;
            case '+': 
            //  ++ operator
                if (tokens[tokens.size() - 1].type == IDENTIFIER && inStream.peek() == '+') { // ++ operator
                    addToken(EQUAL, line, "=");
                    addToken(IDENTIFIER, line, tokens[tokens.size() - 2].lexeme);
                    addToken(PLUS, line, "+");
                    addToken(NUMBER, line, "1.0");
                    c = inStream.get();
                    c = inStream.get();
                    goto SKIP;
                }
                addToken(PLUS, line, "+"); 
            break;
            case '#':
                regexProcessor(&c, line);
                goto SKIP;
            break;
            case '$':
                regexIdentifierProcessor(&c, line);
                goto SKIP;
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
            case '"': string(line); break;
            // Numbers
            default:
                if (isdigit(c)) {
                    number(&c, line, false);
                    goto SKIP;
                } else if (isalpha(c)) {
                    alphaProcessor(&c, line);
                    goto SKIP;
                }
            break;
        }
    }
    return tokens;
}

void Lexer::printLexemes(const std::vector<Token>& tokens) {
    for (int i = 0 ; i < tokens.size() ; i++) {
        if ( tokens[i].type == REGEX || tokens[i].type == IDENTIFIER || this->typeManager->tokenIsRValue(tokens[i].type)) {
            std::cout << tokenToStringMap[tokens[i].type] << " " << tokens[i].lexeme << std::endl;
        } else {
            std::cout << tokenToStringMap[tokens[i].type] << std::endl;        
        }
    }
}

void Lexer::addToken(Tokens type, int line, const std::string& lexeme) {
    struct Token token = {type, line, lexeme};
    tokens.push_back(token);
}

void Lexer::string(int line) {
    std::string literal;
    literal.push_back('\"');
    while (!inStream.eof()) {
        char temp = inStream.get();
        literal.push_back(temp);
        if (temp == '"') {
            literal.push_back('\0');
            addToken(STRING, line, literal);
            return;    
        }
    }
    std::cerr << "Error: String was not closed starting on line: " << line << std::endl;
    exit(1);
}

void Lexer::number (char *c, int line, bool isNegative) {
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

void Lexer::alphaProcessor (char *c , int line) {
    std::string ident; 
    while ( isalpha(*c) || isdigit(*c) ) {
        ident.push_back(*c);
        *c = inStream.get();
    }
    if (stringToTokenMap.count(ident) > 0) {
        addToken(stringToTokenMap[ident], line, ident);
        return;    
    }
    addToken(IDENTIFIER, line, ident);
}

void Lexer::regexProcessor (char *c , int line) {
    std::string regex; 
    regex.push_back(*c);
    *c = inStream.get();
    while (!inStream.eof()) {
        if (!isspace(*c)) {
            regex.push_back(*c);
        }
        if (*c == '#') {
            *c = inStream.get();
            break;
        }
        *c = inStream.get();
    }
    addToken(REGEX, line, regex);
}

void Lexer::regexIdentifierProcessor(char* c, int line) {
    std::string regex; 
    regex.push_back(*c);
    *c = inStream.get();
    while (!inStream.eof()) {
        if (!isspace(*c)) {
            regex.push_back(*c);
        }
        if (*c == '$') {
            *c = inStream.get();
            break;
        }
        *c = inStream.get();
    }
    addToken(IDENTIFIER, line, regex);
}


bool Lexer::previousTokenWas(Tokens t) {
    if (this->tokens[this->tokens.size()-1].type == t) {
        return true;
    }
    return false;
}

bool Lexer::previousTokenWasOperator() {
    for (int i = 0 ; i < (sizeof(OperatorArray)/sizeof(OperatorArray[0])) ; i++) {
        if (previousTokenWas(OperatorArray[i])) {
            return true;
        }
    }
    return false;
}
    



