#include <iostream>
#include <filesystem>
#include <any>
#include <vector>
#include <algorithm>

void recursiveList(std::vector<std::any>& retVec ,const std::filesystem::path& directory, const std::filesystem::path& basePath) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        std::filesystem::path relativePath = std::filesystem::relative(entry, basePath);
        std::string path = relativePath.string();
        retVec.push_back("./" + path);
        if (std::filesystem::is_directory(entry)) {
            recursiveList(retVec, entry, basePath);
        }
    }
}

std::any fLs(std::vector<std::string>& args){
    std::vector<std::string> flags = args;
    std::vector<std::any> retVec; 
    std::string path = "./";
    if (std::find(flags.begin(), flags.end(), std::string("r")) != flags.end()) { /*RECURSIVE FLAG*/
        recursiveList(retVec,path,std::filesystem::absolute(path));
    } else {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            retVec.push_back(path+entry.path().filename().string());
        }
    }
    return std::any(retVec);
}

int main() {
    std::vector<std::string> temp = {"1"};
    std::vector<std::any> sAny = std::any_cast<std::vector<std::any>>(fLs(temp));
    for (int i = 0 ; i < sAny.size() ; i++) {
        std::cout << std::any_cast<std::string>(sAny[i]) << std::endl;
    }
    return 0;
}
