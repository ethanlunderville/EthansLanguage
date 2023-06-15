#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

// Define a Token struct to hold token informationnumber
struct Token {
    TokenType type;
    int line;
    std::string lexeme;
};

// Define a Lexer class
class Lexer {

public:
    Lexer(std::istream& inStream) : inStream(inStream) {}
    // Tokenize the input stream and return a vector of Tokens
    std::vector<Token> scanTokens() {
        //std::vector<Token> tokens;
        int line = 1;
        while (inStream) {
            char c = inStream.get();
            SKIP:
            switch (c) {
                // Single-character tokens
                case '(': addToken(LEFT_PAREN, line, "("); break;
                case ')': addToken(RIGHT_PAREN, line, ")"); break;
                case '{': addToken(LEFT_BRACE, line, "{"); break;
                case '}': addToken(RIGHT_BRACE, line, "}"); break;
                case ',': addToken(COMMA, line, ","); break;
                case '.': addToken(DOT, line, "."); break;
                case '-': addToken(MINUS, line, "-"); break;
                case '+': addToken(PLUS, line, "+"); break;
                case ';': addToken(SEMICOLON, line, ";"); break;
                //case '*': addToken(STAR, line, "*"); break;
                case '*': addToken(inStream.peek() == '*' ? DOUBLESTAR : STAR, line, "*"); break;
                // Two-character tokens
                case '!': addToken(inStream.peek() == '=' ? BANG_EQUAL : BANG, line, "!"); break;
                case '=': addToken(inStream.peek() == '=' ? EQUAL_EQUAL : EQUAL, line, "="); break;
                case '<': addToken(inStream.peek() == '=' ? LESS_EQUAL : LESS, line, "<"); break;
                case '>': addToken(inStream.peek() == '=' ? GREATER_EQUAL : GREATER, line, ">"); break;
                // Comments and slashes
                case '/':
                    if (inStream.peek() == '/') {
                        // A comment goes until the end of the line.
                        while (inStream && inStream.get() != '\n');
                        line++;
                    } else {
                        addToken(SLASH, line, "/");
                    }
                    break;
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
                        number(&c,tokens, line);
                        goto SKIP;
                    } else if (isalpha(c)) {
                        alphaProcessor(&c, tokens, line);
                        goto SKIP;
                    } else {
                        std::cerr << "Error at line " << line << ": unexpected character '" << c << "'\n";
                    }
                    break;
            }
        }
        addToken(EOF_TOKEN, line, "END");
        return tokens;
    }

    void printLexemes(std::vector<Token> tokens) {
        for (int i = 0 ; i < tokens.size() ; i++) {
            if (tokens[i].type == IDENTIFIER || tokens[i].type == NUMBER 
            || tokens[i].type == STRINGTYPE || tokens[i].type == RESERVED ) {
                std::cout << tokenToStringMap[tokens[i].type] << ": " << tokens[i].lexeme << std::endl;
            }
            else {
                std::cout << tokenToStringMap[tokens[i].type] << std::endl;        
            }
        }
    }

private:

    std::istream& inStream;
    std::stringstream currentLexeme;
    std::vector<Token> tokens;
    // Add a token to the vector
    void addToken(TokenType type, int line, std::string lexeme) {
        struct Token token = {type, line, lexeme};
        tokens.push_back(token);
    }
    
    void string(std::vector<Token> tokens , int line) {
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

    void number (char *c, std::vector<Token> tokens , int line) {
        std::string number; 
        while (isdigit(*c)) {
            number.push_back(*c);
            *c = inStream.get();
        }
        addToken(NUMBER, line, number);
        return;
    }

    void alphaProcessor (char *c, std::vector<Token> tokens , int line) {
        std::string ident; 

        while (isalpha(*c)) {
            ident.push_back(*c);
            *c = inStream.get();
        }

        if (ident.compare("int") == 0) {
            addToken(INT, line, ident);
            return;
        } else if (ident.compare("bool") == 0) {
            addToken(BOOL, line, ident);
            return;
        } else if (ident.compare("string") == 0) {
            addToken(STRINGTYPE, line, ident);
            return;
        } 

        if (stringToTokenMap.count(ident) > 0) {
            addToken(stringToTokenMap[ident], line, ident);
            return;    
        }

        addToken(IDENTIFIER, line, ident);

    }

    
};


