#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fstream>

#define SERVER_IP "127.0.0.1"
#define PORT 1602
#define BUFFER_SIZE 1024

bool isClientConnectionClose(char* msg);

bool isSymbolFind(char* msg);

void searchSubstringInFile(char* filePath, char* full_buffer);

int main(int argc, char *argv[]){
    int client;
    char* filename = "ChatHistory.txt";

    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0){
        std::cout << "CLIENT ERROR: socket error";
        exit(0);
    }

    server_address.sin_port = htons(PORT);
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);
    
    std::cout << "\n==> Client socket created";

    int ret = connect(client, reinterpret_cast<const struct sockaddr*>(&server_address), sizeof(server_address));

    if (ret == 0){
        std::cout << "==> Connection to server "
            << inet_ntoa(server_address.sin_addr)
            << " with port number: " << PORT << std::endl;
    }

    char buffer[BUFFER_SIZE];
    std::cout << "Waiting for server confirmation...\n";
    recv(client, buffer, BUFFER_SIZE, 0);
    std::cout << "==> Connection established" 
            << "Enter # to finish the connection\n";

    while (true){
        std::cout << "Client: ";
        std::cin.getline(buffer, BUFFER_SIZE);
        send(client, buffer, BUFFER_SIZE,0);
        if (isClientConnectionClose(buffer)){
                break;
            }
        if (isSymbolFind(buffer)){
            searchSubstringInFile(filename, buffer);
        }

        std::cout << "Server: ";
        recv(client, buffer, BUFFER_SIZE, 0);
        std::cout << buffer << std::endl;
        if (isClientConnectionClose(buffer)){
            break;
        }
        if (isSymbolFind(buffer)){
            searchSubstringInFile(filename, buffer);
        }

    }
    close(client);

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