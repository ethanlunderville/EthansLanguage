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

TypeManager* initTypes(){
    TypeManager* tManage = new TypeManager();
    tManage->addTypeDecl(std::string("string"), STRINGTYPE, String::getInstance());
    tManage->addTypeRVal(STRING, String::getInstance());
    tManage->addTypeDecl(std::string("int"), INT, Number::getInstance());
    tManage->addTypeRVal( NUMBER, Number::getInstance());
    return tManage;
}

int main () {
    TypeManager* typeManager = initTypes();
    ASTPrintVisitor* pVisit = new ASTPrintVisitor();
    std::ifstream file("./test.c");
    Lexer* lexer = new Lexer(file, typeManager);
    Parser* parser = new Parser(lexer, typeManager);
    std::cout << "***PRINTING LEXEMES***" << std::endl; 
    lexer->printLexemes(lexer->scanTokens());
    AST* abstractSyntaxTree = parser->parse();
    ASTChecker* checker = new ASTChecker(typeManager);
    std::cout << "***PRINTING AST***" << std::endl;
    abstractSyntaxTree->accept(pVisit);
    abstractSyntaxTree->accept(checker);
    std::cout << "***PRINTING AST***" << std::endl;
    abstractSyntaxTree->accept(pVisit);
    std::cout << "***INTERPRETING AST***" << std::endl;
    ContextManager* cm = new ContextManager(typeManager);
    ASTInterpreter* interpreter = new ASTInterpreter(typeManager);
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