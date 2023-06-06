class Parser {

    public:
        Parser(Lexer* lexer) {
            this->lexer = lexer;
            this->tokens = this->lexer->scanTokens();
        }

    private:
        int currentTokenIndex;
        Lexer* lexer;
        std::vector<Token> tokens;

        bool isNextToken(TokenType tokenType) {
            if (tokens[currentTokenIndex].type != tokenType) {
                return false;
            }
            return true;
        }

        void scan() {
            this->currentTokenIndex++;
        }

        void expect(TokenType tokenType) {
            if (isNextToken(tokenType)) {
                scan();
                return;
            }
            std::cerr << "COMPILATION TERMINATED" << std::endl;
            std::cerr << "There was a syntax error on line : " << tokens[currentTokenIndex].line << std::endl;
            exit(1);
        }

};