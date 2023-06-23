#include <map>
#include <any>

typedef struct SymbolInfo {
    int line;
    std::any value;
    std::string type;
} SymbolInfo;


class SymbolTable {
    public:
        SymbolTable(SymbolTable* tableReference) : current(0), table(tableReference) {}

        void pushScope() {
            scopeCurr.push(this->currentSize-1);
        }
        void popScope(){
            if (scopeCurr.size() == 0) {
                std::cerr << "EMPTY SCOPE" << std::endl;
            }
            int popCounter = (this->currentSize - 1) - scopeCurr.top();
            scopeCurr.pop();
            for (int i = 0 ; i < popCounter ; i++) {
                stringToSymbolMap.erase(intToStringMap[this->currentSize - 1]);
                intToStringMap.erase(this->currentSize - 1);
                this->currentSize--;
            }
        }
        void declareSymbol(int line, std::string identifier , std::any value, std::string type) {
            if (!(this->contains(identifier) || this->table->contains(identifier))) {
                this->intToStringMap[this->currentSize - 1] = identifier;
                this->stringToSymbolMap[this->intToStringMap[this->currentSize - 1]] = {line, value, type};
            }
            this->currentSize++;
        }
        void reassignSymbol(std::string identifier , std::string value) {

        }

        std::string getTypeOfSymbol(std::string identifier) {
            return "";
        }

        auto getValueStoredInSymbol(std::string identifier) {
            return 1;
        }

        int contains(std::string identifier) {
            for (int i = 0; i < this->currentSize; i++) {
                if (identifier.compare(intToStringMap[i])) {
                    return i;
                }
            }
            return -1;
        }


    private:
        SymbolTable* globalTable;
        int currentSize;
        std::stack<int> scopeCurr;
        std::map<int, std::string> intToStringMap;
        std::map<std::string, SymbolInfo> stringToSymbolMap; 
};