#include "SymbolTable.h"
class ContextManager {
    public:
        ContextManager() {
            this->globalContext = new SymbolTable(nullptr);
        }
        ~ContextManager() {
            while (this->contextStack.size() != 0) {
                delete contextStack.top()
                contextStack.pop()
            }
            delete this->globalContext;
            this->globalContext = nullptr; 
        }
        void ContextManager::pushContext() {
            this->contextStack.push(new SymbolTable(this->globalContext));
        }
        void ContextManager::popContext() {
            if (contextStack.size() > 0) {
                delete contextStack.top()
                contextStack.pop()
                return;
            }
            std::cerr << "Unable to pop empty context stack" << std::endl;
            exit(1);
        }
        void ContextManager::pushScope() {
            if (contextStack.size() > 0) {
                contextStack.top().pushScope();
                return;
            } 
            std::cerr << "No scopes allowed in global memory" << std::endl; 
            exit(1);
        }
        void ContextManager::popScope(){
            if (contextStack.size() > 0) {
                contextStack.top().popScope();
                return;
            } 
            std::cerr << "Unable to pop empty scope stack" << std::endl; 
            exit(1);
        }
        void ContextManager::declareSymbol(int line, std::string identifier , std::any value, std::string type) {  
            if (this->contextStack.size() != 0 && this->contextStack.top().contains(identifier) == -1) {
                this->contextStack.top()->declareSymbol(line, identifier, value, type);
                return;
            } else if (this->globalContext->contains(identifier) == -1) {
                this->globalContext->declareSymbol(line, identifier, value, type);
                return;
            } else {
                std::cerr << 
                    "Multiple declarations of " << identifier << " on line " 
                    << ((potentialLineNum == -1) ? potentialGlobalLineNum : potentialLineNum)
                    << " and line: "
                    << line
                << std::endl;
                exit(1);
            }
        }
        void ContextManager::reassignSymbol(std::string identifier , std::any value) { 
            if (this->contextStack.size() != 0 && this->contextStack.top().contains(identifier) == -1) {
                this->contextStack.top()->reassignSymbol(identifier, value);
                return;
            } else if (this->globalContext->contains(identifier) == -1) {
                this->globalContext->reassignSymbol(identifier, value);
                return;
            } else {
                std::cerr << "Incorrect assignment of undeclared variable: " << identifier << std::endl;
                exit(1);
            }
        }
        std::any ContextManager::getValueStoredInSymbol(std::string identifier) {
            if (this->contextStack.size() != 0 && this->contextStack.top().contains(identifier) == -1) {
                return this->contextStack.top()->getValueStoredInSymbol(identifier);
            } else if (this->globalContext->contains(identifier) == -1) {
                return this->globalContext.top()->getValueStoredInSymbol(identifier);
            } else {
                std::cerr << "Unrecognized identifier: " << identifier << std::endl;
                exit(1);
            }
        }

        std::string ContextManager::getTypeOfSymbol(std::string identifier) {
            if (this->contextStack.size() != 0 && this->contextStack.top().contains(identifier) == -1) {
                return this->contextStack.top()->getTypeOfSymbol(identifier);
            } else if (this->globalContext->contains(identifier) == -1) {
                return this->globalContext.top()->getTypeOfSymbol(identifier);
            } else {
                std::cerr << "Unrecognized identifier: " << identifier << std::endl;
                exit(1);
            }
        }
    private:
        SymbolTable* globalContext;
        std::stack<SymbolTable*> contextStack;
}