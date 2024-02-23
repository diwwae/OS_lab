#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <fcntl.h>


int main(){
    std::fstream file;
    file.open("ChatHistory.txt");
    char buffer[1024]{'A','B','C'};
    file.getline(buffer, 1024);

    file.close();
    // close(file);
    return 0;
}