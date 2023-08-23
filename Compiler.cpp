/*

    File: Compiler.cpp

    Description:
    
    The main driver code of the compiler. Strings 
    together all of the main Compiler components.

    COMPONENTS: 

    *** input -> component -> output ***

    Sourcefile.el -> LEXER -> ProgramTokens 
    ProgramTokens -> PARSER -> AbstractSyntaxTree
    AbstractSyntaxTree -> CHECKER -> CheckedAbstractSyntaxTree
    CheckedAbstractSyntaxTree -> ASTINTERPRETER -> ProgramOutput
    
*/

#include "Lexer.h"
#include "Parser.h"
#include "SymbolTable/ContextManager.h"
#include "SyntaxTree/AST.h"
#include "Visitors/ASTVisitor.h"

/*


                            .__.__                
  ____  ____   _____ ______ |__|  |   ___________ 
_/ ___\/  _ \ /     \\____ \|  |  | _/ __ \_  __ \
\  \__(  <_> )  Y Y  \  |_> >  |  |_\  ___/|  | \/
 \___  >____/|__|_|  /   __/|__|____/\___  >__|   .cpp
     \/            \/|__|                \/       



*/

TypeManager* initPrimitiveTypes(){
    TypeManager* tManage = new TypeManager();
    tManage->addTypeDecl(std::string("string"), STRINGTYPE, String::getInstance()); 
    tManage->addTypeRVal(STRING, String::getInstance());
    tManage->addTypeDecl(std::string("int"), INT, Number::getInstance());
    tManage->addTypeRVal(NUMBER, Number::getInstance());
    return tManage;
}


int main (int argc, char* argv[]) {
    int a = argc;
    if (argc != 2) {
        std::cerr 
        << "ERROR:: Source file not specified. Please speciy the source file as the first argument" 
        << std::endl; 
        exit(1);
    }
    //const char* execPath = "Examples/test3.c\n";
    //std::ifstream file("Examples/test3.c");
    std::string sourceFile = std::string(argv[1]);
    std::ifstream file(sourceFile);
    if (!file.is_open()) {
        std::cerr 
        << "ERROR:: Source file "
        << sourceFile
        << " could not be opened" 
        << std::endl; 
        exit(1);
    }
    Builtins::setFilePath(argv[1]);
    TypeManager* typeManager = initPrimitiveTypes();
    ASTPrintVisitor* pVisit = new ASTPrintVisitor();
    Lexer* lexer = new Lexer(file, typeManager);
    Parser* parser = new Parser(lexer, typeManager);
    #ifdef INTERPRETEROUTPUT
        #ifdef PRINTTOKENS
            std::cout << "***PRINTING LEXEMES***" << std::endl;  
            lexer->printLexemes(std::move(lexer->scanTokens()));
        #endif
    #endif
    AST* abstractSyntaxTree = parser->parse();
    ASTChecker* checker = new ASTChecker(typeManager);
    #ifdef INTERPRETEROUTPUT
        #ifdef PRINTTREE
            std::cout << "***PRINTING AST***" << std::endl;
            abstractSyntaxTree->accept(pVisit);
        #endif
        std::cout << "***CHECKING AST***" << std::endl;
    #endif
    abstractSyntaxTree->accept(checker);
    #ifdef INTERPRETEROUTPUT
        #ifdef PRINTTREE
            std::cout << "***PRINTING AST***" << std::endl;
            abstractSyntaxTree->accept(pVisit);
        #endif   
        std::cout << "***INTERPRETING AST***" << std::endl; 
    #endif
    ASTInterpreter* interpreter = new ASTInterpreter(typeManager);
    abstractSyntaxTree->accept(interpreter);
    #ifdef PRINTDEALLOCATIONADDRESSES
        std::cout << "***DEALLOCATING AST***" << std::endl;
    #endif
    ASTDeallocationVisitor* dVisit = new ASTDeallocationVisitor();
    abstractSyntaxTree->accept(dVisit);

    delete checker;
    checker = nullptr;
    delete typeManager;
    typeManager = nullptr;
    delete pVisit;
    pVisit = nullptr;
    delete parser;
    parser = nullptr;
    delete lexer;
    lexer = nullptr;
    delete dVisit;
    dVisit = nullptr;
    delete interpreter;
    interpreter = nullptr;

    return 0;
}