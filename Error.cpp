#include "Error.h"

Error::Error(std::string sourceFilePath) {
    std::ifstream inputFile(sourceFilePath);
    if (!inputFile.is_open()) {
        std::cerr 
        << "Failed to open the source file file." 
        << std::endl;
        exit(1);
    }
    this->buffer << inputFile.rdbuf();
    inputFile.close();
}

void Error::addError(std::string message, ERRORTYPES eType, int line) {
    this->messageToType[message] = eType;
    this->messageToLine[message] = line;
}

void Error::throwError() {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(this->buffer, line)) {
        lines.push_back(line);
    }
    for (const auto& pair : messageToLine) {
        int lineToHandle = messageToLine[pair.first] - 2;
        std::cerr 
        << errorStrings[messageToType[pair.first]] 
        << ":: " 
        << pair.first
        << std::endl;
        if (lineToHandle < 0) {
            lineToHandle = 0;
        }
        int start = lineToHandle;
        for (int i = lineToHandle ; i < lineToHandle + 5 && i < lines.size() ; i++) {
            std::cerr << "  " << i << "  |\t";
            if (i - start == 2) {
                std::cerr << "\033[1m" 
                << lines[i] 
                << "\033[0m"; 
                std::cerr<< "\033[1;31m" 
                << " <-- ERROR" 
                << "\033[0m" 
                << std::endl;
            } else {
                std::cerr << lines[i] << std::endl;
            }
        }
    }
    exit(1);
}