#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"
#define PORT 1602
#define BUFFER_SIZE 1024

bool is_client_connection_close(char* msg);

int main(int argc, char *argv[]){
    int client;

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
        if (is_client_connection_close(buffer)){
                break;
            }

        std::cout << "Server: ";
        recv(client, buffer, BUFFER_SIZE, 0);
        std::cout << buffer << std::endl;
        if (is_client_connection_close(buffer)){
            break;
        }

    }
    close(client);

    return 0;
}

bool is_client_connection_close(char* msg){
    for(int i=0; i < strlen(msg); ++i){
        if (msg[i] == '#'){
            return true;
        }
    }
    return false;
}