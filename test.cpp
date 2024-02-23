#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <fcntl.h>

void searchSubstringInFile(char* filePath, char* buffer) {
    std::ifstream file(filePath);
    std::string line;
    bool flag = false;

    while (std::getline(file, line)) {
        std::cout << line;
        if (std::strstr(line.c_str(), buffer) != nullptr) {
            std::cout << "Substring found: " << line << std::endl;
            flag = true;
        }
        if (flag == true) {
            return;
        }
    }

    std::cout << "Substring not found in the file." << std::endl;
    return;
}

int main(){
    // std::fstream file;
    char *filename = "ChatHistory.txt";
    char *input = "hello";
    searchSubstringInFile(filename, input);

    char *buffer = "@hello";
    char *substring = buffer + 1;
    std::cout << substring;
    // FILE *file = fopen("ChatHistory.txt", "ab+");
    // char buffer[1024]{'V','B','C'};
    // fprintf(file, buffer);
    // fprintf(file, "\n");
    // fprintf(file, buffer);
    // file.getline(buffer, 1024);
    // file.close();
    // close(file);
    return 0;
}