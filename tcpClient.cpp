#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 4444

int main() {
    int clientSocket, ret; //client socket and return value
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);  //create a socket
    if (clientSocket < 0) {
        std::cout << "[-]Error in connection." << std::endl;  //error in connection
        exit(1);
    }
    std::cout << "[+]Client Socket is created." << std::endl;  //client socket created

    memset(&serverAddr, 0, sizeof(serverAddr));  //clear the server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //set the server address

    ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)); //connect to the server
    if (ret < 0) {  //error in connection
        std::cout << "[-]Error in connection." << std::endl;
        exit(1);
    }
    std::cout << "[+]Connected to Server." << std::endl;  //connected to server

    while (true) {
        std::cout << "Client: \t";
        std::cin >> buffer;
        send(clientSocket, buffer, strlen(buffer), 0);    //send the message to the server

        if (strcmp(buffer, ":exit") == 0) {  //exit the program
            close(clientSocket);
            std::cout << "[-]Disconnected from server." << std::endl;  //disconnected from server
            exit(1);
        }

        if (recv(clientSocket, buffer, 1024, 0) < 0) {  //receive the message from the server
            std::cout << "[-]Error in receiving data." << std::endl;
        } else {
            std::cout << "Server: \t" << buffer << std::endl;  //print the message from the server
        }
    }

    return 0;
}
