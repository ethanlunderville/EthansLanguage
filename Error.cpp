enum ERRORTYPES {
    SCANNINGERROR, 
    PARSINGERROR, 
    SYMTABLEERROR, 
    CHECKERROR, 
    RUNTIMEERROR
};

static std::map<ERRORTYPES, std::strings> errorStrings = {
    {SCANNINGERROR, "\033[1;31mSCANNINGERROR\033[0m\n"},
    {PARSINGERROR, "\033[1;31mPARSINGERROR\033[0m\n"},
    {SYMTABLEERROR, "\033[1;31mSYMTABLEERROR\033[0m\n"},
    {CHECKERROR, "\033[1;31mCHECKERROR\033[0m\n"},
    {RUNTIMEERROR, "\033[1;31mRUNTIMEERROR\033[0m\n"},
}

class Error {
public:
    Error(std::string sourceFilePath) {
        std::ifstream inputFile(sourceFilePath);
        if (!inputFile.is_open()) {
            std::cerr 
            << "Failed to open the source file file." 
            << std::endl;
            return 1;
        }
        this->buffer << inputFile.rdbuf();
        inputFile.close();
    }
    void addError(std::string message, ERRORTYPES eType, int line) {
        this->errorMessages[message] = eType;
        this->messageToLine[message] = line;
    }
    throwError() {
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(this->buffer, line)) {
            lines.push_back(line);
        }
        for (const auto& pair : messageToLine) {
            int lineToHandle = messageToLine[pair.first];
            std::cerr 
            << errorStrings[messageToType[pair.first]] 
            << ":: " 
            << pair.first
            << "\n"
            << std::endl;
            for (int i = lineToHandle ; i < lineToHandle + 5 && i < lines.size() ; i++) {
                std::cerr << lines[i] << std::endl;
            }
        }
        exit(1);
    }
private:
    std::stringstream buffer;
    std::map<std::string&, ERRORTYPES> messageToType;
    std::map<std::string&, int> messageToLine;    
};
/*
int main () {
    Error* newErr = new Error("./test.c");
    newErr->addError("There was an issue", SCANNINGERROR, 2);
    newErr->throwError();
    return 0;
}
*/