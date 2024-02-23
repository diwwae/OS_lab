#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <fstream>

#define PORT 1602
#define BUFFER_SIZE 1024

bool isClientConnectionClose(char* msg);

bool isSymbolFind(char* msg);

void searchSubstringInFile(char* filePath, char* buffer);

int main(int argc, char *argv[]){
    // int file = open("ChatHistory.txt", O_WRONLY | O_CREAT, 0777);
    // std::fstream file("ChatHistory.txt");
    // file.open("ChatHistory.txt");
    // file << "gqw";
    // if (!file.is_open()){
    //     std::cout << "ERROR: can't open file";
    //     exit(0);
    // }

    // dup2(file,1);
    char* filename = "ChatHistory.txt"; 
    FILE *file = fopen(filename, "ab+");

    int client;
    int server;

    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0){
        std::cout << "SERVER ERROR: socket error";
        exit(0);
    }

    std::cout << "SERVER: Socket was succesfully created\n";

    server_address.sin_port = htons(PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    int ret = bind(client, reinterpret_cast<struct sockaddr*>(&server_address),
        sizeof(server_address));

    if (ret < 0){
        std::cout << "SERVER ERROR: binding connection. Socket has already been establishing.\n";
        return -1;
    }

    socklen_t size = sizeof(server_address);
    std::cout << "SERVER: Listening client...";
    listen(client, 1);

    server = accept(client, reinterpret_cast<struct sockaddr*>(&server_address), &size);
    if (server < 0){
        std::cout << "SERVER ERROR: Can't accept to client";
    }
    
    char buffer[BUFFER_SIZE];
    bool isExit = false;
    while(server > 0){
        strcpy(buffer, "==> Server connected\n");
        send(server, buffer, BUFFER_SIZE, 0);
        std::cout << "==> Connected to the client\n" 
            << "Enter # to finish the connection\n";

        std::cout << "Client: ";
        recv(server, buffer, BUFFER_SIZE, 0); 
        std::cout << buffer << std::endl;
        if (isClientConnectionClose(buffer))      
            isExit = true;
    

        while (!isExit){
            std::cout << "Server: ";
            std::cin.getline(buffer, BUFFER_SIZE);

            // file.write(buffer, BUFFER_SIZE);
            // file << "hi";
            // file.getline(buffer, BUFFER_SIZE);
            // write(file, buffer, BUFFER_SIZE);
            fprintf(file, "Server: ");
            fprintf(file, buffer);
            fprintf(file, "\n");
            send(server, buffer, BUFFER_SIZE, 0);
            if (isClientConnectionClose(buffer)){
                break;
            }
            if (isSymbolFind(buffer)){
                searchSubstringInFile(filename, buffer);
            }
            
            std::cout << "Client: ";
            recv(server, buffer, BUFFER_SIZE, 0);
            std::cout << buffer << std::endl;
            fprintf(file, "Client: ");
            fprintf(file, buffer);
            fprintf(file, "\n");
            if (isClientConnectionClose(buffer)){
                break;
            }
            if (isSymbolFind(buffer)){
                searchSubstringInFile(filename, buffer);
            }
        }
        std::cout << "Finished...\n";
        isExit = false;
        exit(0);
    }
    close(server);
    // close(file);
    fclose(file);
    return 0;
}

bool isClientConnectionClose(char* msg){
    for(int i=0; i < strlen(msg); ++i){
        if (msg[i] == '#'){
            return true;
        }
    }
    return false;
}

bool isSymbolFind(char* msg){
    for(int i=0; i < strlen(msg); ++i){
        if (msg[i] == '@'){
            return true;
        }
    }
    return false;
}

void searchSubstringInFile(char* filePath, char* full_buffer) {
    std::ifstream file(filePath);
    std::string line;
    bool flag = false;
    char *buffer = full_buffer + 1;

    while (std::getline(file, line)) {
        if (std::strstr(line.c_str(), buffer) != nullptr) {
            std::cout << "Substring found: " << line << std::endl;
            flag = true;
        }
    }
    if (flag == true) {
        return;
    }

    std::cout << "Substring not found in the file." << std::endl;
    return;
}