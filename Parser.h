class Parser {

    public:
    
        Parser(Lexer* lexer);
        ~Parser();

        AST* sProgram();
        
        AST* sStatement();
        AST* sIf();
        AST* sElse();
        AST* sWhile();
        AST* sReturn();
        AST* sExpression();
        AST* sBlock();

        AST* sDeclaration();
        AST* sFunctionDeclaration();
        AST* sFunction();
        AST* sAssign();

        AST* sAdd();
        AST* sDivide();
        AST* sMultiply();
        AST* sSubtract();

    private:

        int currentTokenIndex;
        Lexer* lexer;
        std::vector<Token> tokens;
        std::vector<AST*> flatTreeHolder; // References to AST nodes are held to easily free the nodes

        bool onStatement();
        bool onDeclaration();
        bool isCurrentToken(int tokenType);
        void registerNode(AST* node);
        void scan();
        void expect(TokenType tokenType);

};