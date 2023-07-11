/*

    File: Compiler.cpp

    Description:
    
    The main driver code of the compiler. Strings 
    together all of the main Compiler components.

    COMPONENTS: 

    *** input -> component -> output ***

    Sourcefile.el -> LEXER -> ProgramTokens 
    ProgramTokens -> PARSER -> AbstractSyntaxTree
    if [TREEWALKER] 
        AbstractSyntaxTree -> TREEWALKER -> ProgramOutput

    
*/
//            declString -> tokenType
// string ->  tokenType -> ASTNode -> TypeHandler

#include "Lexer.h"
#include "Parser.h"
#include "SymbolTable/ContextManager.h"
#include "Visitors/ASTVisitor.h"

int main () {

    TypeManager* typeManager = new TypeManager();
    ASTPrintVisitor* pVisit = new ASTPrintVisitor();
    std::ifstream file("./test.c");
    Lexer* lexer = new Lexer(file);
    Parser* parser = new Parser(lexer, typeManager);
    std::cout << "***PRINTING LEXEMES***" << std::endl; 
    lexer->printLexemes(lexer->scanTokens());
    AST* abstractSyntaxTree = parser->parse();
    ASTChecker* checker = new ASTChecker();
    std::cout << "***PRINTING AST***" << std::endl;
    abstractSyntaxTree->accept(pVisit);
    abstractSyntaxTree->accept(checker);
    std::cout << "***PRINTING AST***" << std::endl;
    abstractSyntaxTree->accept(pVisit);
    std::cout << "***INTERPRETING AST***" << std::endl;
    ContextManager* cm = new ContextManager();
    ASTInterpreter* interpreter = new ASTInterpreter();
    //LET THE FUN BEGIN
    abstractSyntaxTree->accept(interpreter);
    std::cout << "***DEALLOCATING AST***" << std::endl;
    ASTDeallocationVisitor* dVisit = new ASTDeallocationVisitor();
    abstractSyntaxTree->accept(dVisit);
    delete typeManager;
    typeManager = nullptr;
    delete cm;
    cm = nullptr;
    delete pVisit;
    pVisit = nullptr;
    delete parser;
    parser = nullptr;
    delete lexer;
    lexer = nullptr;
    delete dVisit;
    dVisit = nullptr;
    return 0;
}