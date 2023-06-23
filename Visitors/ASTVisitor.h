/*

    File: ASTVisitor.h

    Description:

    This is the header file for the ASTVisitor and all of
    its subclasses. 

    Notes:

    I decided to use the Visitor design pattern because 
    I felt that it would be beneficial to have the 
    functions grouped by functionality instead of types.
    What I mean by this is that, I could have created a
    virtual function in the AST class
    so that all of the AST subclasses would have to 
    implement the function in their own way (polymorphism)
    and then used dynamic binding to determine which 
    function would actually be executed during runtime.
    The issue with this would be that I would have to 
    open the file for every subclass and implement that 
    function every time I wanted to implement a new piece
    of functionality that all AST nodes should have. 
    
    From the outset of the project I knew there would be
    a huge ammount of AST classes so the above approach
    seemed to be too cumbersome.

    The Visitor pattern allows the programmer to store
    all of the functionality (of one kind of function) 
    for many different types in one place instead of 
    storing each implementation of that function in
    the functions class as is the default approach
    in Object Oriented Programming.

    For example: Instead of defining a specialized 
    print() function in every AST subclass. I can 
    define all of the print() functions in one file.

    The downside to this approach is that everytime
    a new type is added, all of the visitors have to
    be updated to acccomodate that type. 

    Nonetheless, the positives of the Visitor pattern
    outweigh the negatives for this project.
    
*/
#ifndef ASTVISITOR_H
#define ASTVISITOR_H
#include <iostream>
class AST;
/*Abstract*/ class ASTVisitor {
    public:
        virtual void visitChildren(AST* astree)=0;
        virtual void visitAssignTree(AST* astree)=0;
        virtual void visitIfTree(AST* astree)=0;
        virtual void visitExpressionTree(AST* astree)=0;
        virtual void visitDivideTree(AST* astree)=0;
        virtual void visitMultiplyTree(AST* astree)=0;
        virtual void visitAddTree(AST* astree)=0;
        virtual void visitSubtractTree(AST* astree)=0;
        virtual void visitExponentTree(AST* astree)=0;
        virtual void visitDeclarationTree(AST* astree)=0;
        virtual void visitBlockTree(AST* astree)=0;
        virtual void visitReturnTree(AST* astree)=0;
        virtual void visitProgramTree(AST* astree)=0;
        virtual void visitFunctionTree(AST* astree)=0;
        virtual void visitFunctionDeclarationTree(AST* astree)=0;
        virtual void visitWhileTree(AST* astree)=0;
        virtual void visitElseTree(AST* astree)=0;
        virtual void visitStringTree(AST* astree)=0;
        virtual void visitNumberTree(AST* astree)=0;
        virtual void visitGreaterTree (AST* astree)=0;
        virtual void visitGreaterEqualTree (AST* astree)=0;
        virtual void visitLessTree (AST* astree)=0;
        virtual void visitLessEqualTree (AST* astree)=0;
        virtual void visitEqualTree (AST* astree)=0;
        virtual void visitNotEqualTree (AST* astree)=0;
        virtual void visitAndTree (AST* astree)=0;
        virtual void visitOrTree (AST* astree)=0;
};

class ASTPrintVisitor: public ASTVisitor {
    public:
        ASTPrintVisitor();
        //INHERITED FUNCTIONS
        void visitChildren(AST* astree) override;
        void visitAssignTree (AST* astree) override;
        void visitIfTree (AST* astree) override;
        void visitExpressionTree (AST* astree) override;
        void visitDivideTree (AST* astree) override;
        void visitMultiplyTree (AST* astree) override;
        void visitAddTree (AST* astree) override;
        void visitSubtractTree (AST* astree) override;
        void visitDeclarationTree (AST* astree) override;
        void visitBlockTree (AST* astree) override;
        void visitReturnTree (AST* astree) override;
        void visitProgramTree (AST* astree) override;
        void visitFunctionTree (AST* astree) override;
        void visitFunctionDeclarationTree (AST* astree) override;
        void visitWhileTree (AST* astree) override;
        void visitElseTree (AST* astree) override;
        void visitStringTree(AST* astree) override;
        void visitNumberTree(AST* astree) override;
        void visitExponentTree(AST* astree) override;
        void visitGreaterTree (AST* astree) override;
        void visitGreaterEqualTree (AST* astree) override;
        void visitLessTree (AST* astree) override;
        void visitLessEqualTree (AST* astree) override;
        void visitEqualTree (AST* astree) override;
        void visitNotEqualTree (AST* astree) override;
        void visitAndTree (AST* astree) override;
        void visitOrTree (AST* astree) override;
        //Visitor specific functions
        void printIndent();
        void printer(std::string type, AST* node, std::string symbol);
        void printer(std::string type, AST* node);
    private:
        int indent;
        int lineNum;
};

class ASTDeallocationVisitor: public ASTVisitor {
    public:
        ASTDeallocationVisitor();
        //INHERITED FUNCTIONS
        void visitChildren(AST* astree) override;
        void visitAssignTree (AST* astree) override;
        void visitIfTree (AST* astree) override;
        void visitExpressionTree (AST* astree) override;
        void visitDivideTree (AST* astree) override;
        void visitMultiplyTree (AST* astree) override;
        void visitAddTree (AST* astree) override;
        void visitSubtractTree (AST* astree) override;
        void visitDeclarationTree (AST* astree) override;
        void visitBlockTree (AST* astree) override;
        void visitReturnTree (AST* astree) override;
        void visitProgramTree (AST* astree) override;
        void visitFunctionTree (AST* astree) override;
        void visitFunctionDeclarationTree (AST* astree) override;
        void visitWhileTree (AST* astree) override;
        void visitElseTree (AST* astree) override;
        void visitStringTree(AST* astree) override;
        void visitNumberTree(AST* astree) override;
        void visitExponentTree(AST* astree) override;
        void visitGreaterTree (AST* astree) override;
        void visitGreaterEqualTree (AST* astree) override;
        void visitLessTree (AST* astree) override;
        void visitLessEqualTree (AST* astree) override;
        void visitEqualTree (AST* astree) override;
        void visitNotEqualTree (AST* astree) override;
        void visitAndTree (AST* astree) override;
        void visitOrTree (AST* astree) override;
        //Deallocator specific functions
        void deAllocateChildren();
        void deallocate(AST* node);
    private:
        int currentNodeNum;
};

#endif