#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include "server_api.h"

#define PORT 5020  // Use any unused port

int main() {
    int server_fd, new_socket, server_bind;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[8192] = {0};

    ServerAPI api;

    // 1. Create socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind the socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (::bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    while (true) {
    // 4. Accept connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        continue;
    }

    std::cerr << "Client connected." << std::endl;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, 8192);
        
        if (valread <= 0) {
            std::cerr << "Client disconnected or error\n";
            break;
        }

        std::string requestStr(buffer, valread);  // preserve only read bytes
        std::string responseStr = api.requestHandler(requestStr);

        std::cerr << "Request Handler successfully executed" << std::endl;

        // Optionally send response
        // send(new_socket, responseStr.c_str(), responseStr.size(), 0);
    }

    close(new_socket);  // Close connection when client is done
    std::cerr << "Running the loop again\n";
    }
    
    return 0;
}
