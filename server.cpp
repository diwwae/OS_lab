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

#define PORT 1604
#define BUFFER_SIZE 1024

bool isClientConnectionClose(char* msg);

bool isSymbolFind(char* msg);

void searchSubstringInFile(char* filePath, char* buffer);

void printToFile(char* user_name, char *buffer);

int main(int argc, char *argv[]){

    char* filename = "ChatHistory.txt"; 
    // FILE *file = fopen(filename, "ab+");

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
        // fprintf(file, "Client: ");
        // fprintf(file, buffer);
        // fprintf(file, "\n");
        printToFile("Client", buffer);
        if (isClientConnectionClose(buffer))      
            isExit = true;
        if (isSymbolFind(buffer)){
            searchSubstringInFile(filename, buffer);
        }
        
    

        while (!isExit){
            std::cout << "Server: ";
            std::cin.getline(buffer, BUFFER_SIZE);
            // fprintf(file, "Server: ");
            // fprintf(file, buffer);
            // fprintf(file, "\n");
            printToFile("Server", buffer);
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
            // fprintf(file, "Client: ");
            // fprintf(file, buffer);
            // fprintf(file, "\n");
            printToFile("Client", buffer);
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
    // fclose(file);
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

            // char full_msg[256]{};
            // std::cout << "....................................." << std::endl;
            // std::strcpy(full_msg, "Substring found: " ); 
            // std::cout << "||||||||||||||||||||||||||||||||||||||||" << std::endl;
            // std::strcat(full_msg, line.c_str());
            // std::cout << "....................................." << std::endl;
            // printToFile("system", full_msg);
            // std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1!" << std::endl;
            flag = true;
        }
    }
    if (flag == true) {
        return;
    }

    std::cout << "Substring not found in the file." << std::endl;
    return;
}

void printToFile(char* user_name, char *buffer){
    char* filename = "ChatHistory.txt"; 
    FILE *file = fopen(filename, "ab+");

    // if (user_name != "system"){
    //     fprintf(file, user_name);
    //     fprintf(file, ": ");
    // }

    fprintf(file, user_name);
    fprintf(file, ": ");
    fprintf(file, buffer);
    fprintf(file, "\n");

    fclose(file);
    return;
}