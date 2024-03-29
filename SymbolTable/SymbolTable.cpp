#include "SymbolTable.h"

SymbolTable::SymbolTable() :  referenceCount(1), returned(false) {
    this->tableReference = nullptr;
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

void SymbolTable::declareSymbol(int line, std::string identifier, Type* typeHandler) {
    this->intToStringVector.push_back(identifier);
    if (dynamic_cast<PrimitiveType*>(typeHandler) == nullptr && typeid(*typeHandler) != typeid(Struct)) { // If the type is not primitve then it must be manually deallocated
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

std::any* SymbolTable::getReferenceOfValueStoredInSymbol(std::string identifier) {
    return &(this->stringToSymbolMap[identifier].value);
}

std::any SymbolTable::getValueStoredInSymbol(std::string identifier, int subscript) {
    if (this->stringToSymbolMap[identifier].value.type() == typeid(std::string)) {
        std::string anyString = std::any_cast<std::string>(this->stringToSymbolMap[identifier].value);
        return std::string(1,anyString[subscript]);
    }
    std::vector<std::any> anyVec = std::any_cast<std::vector<std::any>>(this->stringToSymbolMap[identifier].value);
    return anyVec[subscript];
} 

std::any* SymbolTable::getReferenceOfValueStoredInSymbol(std::string identifier, int subscript) {
    if (this->stringToSymbolMap[identifier].value.type() == typeid(std::string)) {
        std::any& actualValue = this->stringToSymbolMap[identifier].value;
        char* charPointer = &(std::any_cast<std::string>(actualValue)[subscript]);
        std::any returner = std::any(charPointer);
        std::any* returnPointer = &returner;
        return returnPointer;
        //std::cerr << "Unable to assign string subscripts" << std::endl;
        //exit(1);
    }
    std::any actualValue = std::move(this->stringToSymbolMap[identifier].value);
    std::any* returnPointer = &(std::any_cast<std::vector<std::any>>(actualValue)[subscript]);
    this->stringToSymbolMap[identifier].value = actualValue;
    return returnPointer;
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
    if (typeHandler->checkType(value)) { 
        return true;
    }
    return false;
}

void SymbolTable::setCurrentFunctionType(Type* t) {
    this->functionType = t;
}
Type* SymbolTable::getCurrentFunctionType() {
    return this->functionType;
}

std::string& SymbolTable::getTypeName() {
    return this->typeName;
}

std::string& SymbolTable::getStructName() {
    return this->structName;
}

void SymbolTable::setTypeName(const std::string& name) { this->typeName = name; }

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

 
