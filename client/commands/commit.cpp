/*
 * Author: Dev Rathod
 * Email: devmanojrathod@gmail.com
 * Date: 31st July, 2025
 * 
 * This file executes the commit command to upload files onto the version control server
 * 
 * */
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include "commit.h"
#include "../network_client.h"  // for sendJsonRequest()
#include "../auth.h"            // for loadTokenAndUsername()


#include <thread>
#include <chrono>

#define PORT 6969

using json = nlohmann::json;
using namespace std;
using namespace std::filesystem;

// const int port = 9001;


/*
 * function name: isValidToken
 * Creation Date: 31st July, 2025
 * Author: Dev Rathod
 *
 * This function is used to send the commit file to the server in form of binary 
 * after establishing a socket communication
 *  
 */
bool commit::sendFileToServer(const std::string& ip, int port, const std::string& username,
                              const std::string& repoName, const std::string& filePath) {
    // Establishing a socket connection with the server for file transfer
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[client] Socket creation failed");
        return false;
    }

    sockaddr_in servAddr{};
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);

    // Converting the IP Address string into a binary network format
    if (inet_pton(AF_INET, ip.c_str(), &servAddr.sin_addr) <= 0) {
        cerr << "[client] Invalid address or address not supported.\n";
        close(sock);
        return false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    bool connected = false;
    for (int i = 0; i < 10; ++i) {  // Try for ~5 seconds
        if (connect(sock, (sockaddr*)&servAddr, sizeof(servAddr)) == 0) {
            cout << "[client] ✅ Connected to file server at " << ip << ":" << port << "\n";
            connected = true;
            break;
        } else {
            perror("[client] Connect attempt");
            std::cerr << "[client] Retrying connection to file server...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    if (!connected) {
        cerr << "[client] ❌ Connection to file server failed after retries.\n";
        close(sock);
        return false;
    }

    // Send file metadata header
    string filename = path(filePath).filename().string();
    string header = username + "\n" + repoName + "\n" + filename + "\n";
    cerr<<"File Name:"<<filename<<endl;
    cerr<<"Ready to send String:"<<header.c_str();
    send(sock, header.c_str(), header.size(), 0);

    // Sending binary content
    ifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        cerr << "[client] ❌ Could not open file: " << filePath << "\n";
        close(sock);
        return false;
    }

    char buffer[4096];

    cerr<<"Reading the file line by line"<<endl;
    while (file.read(buffer, sizeof(buffer))) {
        send(sock, buffer, file.gcount(), 0);
    }
    cerr<<"Sending the file line by line"<<endl;

    // Send any remaining bytes
    if (file.gcount() > 0) {
        send(sock, buffer, file.gcount(), 0);
    }

    file.close();
    close(sock);
    return true;
}


/*
 * function name: getUsernameFromToken
 * Creation Date: 31st July, 2025
 * Author: Dev Rathod
 * 
 * This function excecutes the commitCommand and processes the files for uploading
 */
void commit::commitCommand(const std::string& repoName, const std::string& filePath) {
    // To store the token and username
    string token, username;
    auth.loadTokenAndUsername(token, username); // auth.h functionality

    cerr<<"Everything fine before json"<<endl;
    // Send JSON commit request
    json request = {
        {"type", "commit"},
        {"command", "commit"},
        {"token", token},
        {"repo_name", repoName},
        {"filename", path(filePath).filename().string()}
    };
    cerr<<"Everything fine after json"<<endl;
    cerr<<"Request:" << request.dump()<<endl;
    cerr << "[client] Sending file...\n";
    if (sendFileToServer("127.0.0.1", PORT, username, repoName, filePath)) {
        cout << "[client] Commit uploaded successfully.\n";
    } else {
        cerr << "[client] Commit upload failed.\n";
    }
}