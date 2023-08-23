#include <map>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>

enum ERRORTYPES {
    ERROR,
    COMPILEERROR,
    SCANNINGERROR, 
    PARSINGERROR, 
    SYMTABLEERROR, 
    CHECKERROR, 
    RUNTIMEERROR
};

static std::map<ERRORTYPES, std::string> errorStrings = {
    {ERROR, "\033[1;31mERROR\033[0m\n"},
    {COMPILEERROR, "\033[1;31mCOMPILEERROR\033[0m\n"},
    {SCANNINGERROR, "\033[1;31mSCANNINGERROR\033[0m\n"},
    {PARSINGERROR, "\033[1;31mPARSINGERROR\033[0m\n"},
    {SYMTABLEERROR, "\033[1;31mSYMTABLEERROR\033[0m\n"},
    {CHECKERROR, "\033[1;31mCHECKERROR\033[0m\n"},
    {RUNTIMEERROR, "\033[1;31mRUNTIMEERROR\033[0m\n"}
};

class Error {
public:
    Error(std::string sourceFilePath);
    void addError(std::string message, ERRORTYPES eType, int line);
    void throwError();
private:
    std::stringstream buffer;
    std::map<std::string, ERRORTYPES> messageToType;
    std::map<std::string, int> messageToLine;    
};


