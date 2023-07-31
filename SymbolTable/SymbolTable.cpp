#include "SymbolTable.h"

SymbolTable::SymbolTable(SymbolTable* tableReference) : tableReference(tableReference) {
    pushScope();
    this->referenceCount = 1;
}

SymbolTable::~SymbolTable() {
    for (int i = 0 ; i < this->deletableTypes.size() ; i++) {
        delete this->deletableTypes[i];
        this->deletableTypes[i] = nullptr;
    }
    for (int i = 0 ; i < this->structs.size() ; i++) {
        if (this->structs[i]->getReferenceCount() == 1) {
            delete this->structs[i];
            this->structs[i] = nullptr;
        } else {
            this->structs[i]->decrementReferenceCount();
        }
    }
}

void SymbolTable::pushScope() {
    scopeStack.push(this->getCurrentSize());
}

void SymbolTable::popScope(){
    if (scopeStack.size() == 0) {
        std::cerr << "Cannot pop an empty scope" << std::endl;
    }
    int popCounter = this->getCurrentSize() - scopeStack.top();
    scopeStack.pop();
    for (int i = 0 ; i < popCounter ; i++) {
        stringToSymbolMap.erase(intToStringVector[this->getCurrentSize() - 1]);
        intToStringVector.erase(intToStringVector.begin() + (this->getCurrentSize() - 1));
    }
}

void SymbolTable::declareSymbol(int line, std::string identifier, Type* typeHandler) {
    this->intToStringVector.push_back(identifier);
    if (dynamic_cast<PrimitiveType*>(typeHandler) == nullptr) { // If the type is not primitve then it must be manually deallocated
        this->deletableTypes.push_back(typeHandler);
    }
    this->stringToSymbolMap[this->intToStringVector[this->getCurrentSize() - 1]] = {line, typeHandler->getNullValue(), typeHandler};
    if (typeid(typeHandler) == typeid(Struct)) {
        this->structs.push_back(std::any_cast<SymbolTable*>(this->stringToSymbolMap[this->intToStringVector[this->getCurrentSize() - 1]].value));
    }
}

void SymbolTable::reassignSymbol(std::string identifier , std::any value) {  
    this->stringToSymbolMap[identifier].value = value;
}

Type* SymbolTable::getTypeOfSymbol(std::string identifier) {
    return this->stringToSymbolMap[identifier].type;
}

std::any SymbolTable::getValueStoredInSymbol(std::string identifier) {
    return this->stringToSymbolMap[identifier].value;
}

int SymbolTable::getCurrentSize() {
    return this->intToStringVector.size();
}

int SymbolTable::contains(std::string identifier) {
    for (int i = 0; i < this->getCurrentSize(); i++) {
        if (identifier.compare(intToStringVector[i]) == 0) {
            return i;
        }
    }
    return -1;
}

bool SymbolTable::variableTypeCheck(Type* typeHandler , std::any value) {
    if (typeHandler->checkType(value)) { return true; }
    return false;
}

void SymbolTable::setCurrentFunctionType(Type* t) {
    this->functionType = t;
}
Type* SymbolTable::getCurrentFunctionType() {
    return this->functionType;
}

std::string& SymbolTable::getStructName() {
    return this->structName;
}

void SymbolTable::setStructName(const std::string& name) { this->structName = name; }

int SymbolTable::getReferenceCount() {return this->referenceCount;}

void SymbolTable::incrementReferenceCount() {this->referenceCount++;}

void SymbolTable::decrementReferenceCount() {this->referenceCount--;}

void SymbolTable::printSymbolTable() {
    for (int i = (intToStringVector.size()-1) ; i > -1 ; i--) {
        stringToSymbolMap[intToStringVector[i]].type
        ->printSymbol(intToStringVector[i], stringToSymbolMap[intToStringVector[i]].value);
    }
}

 
