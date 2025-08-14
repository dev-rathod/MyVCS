/*
 * Author: Dev Rathod
 * Email: devmanojrathod@gmail.com
 * Date: 31st July, 2025
 * 
 * This file executes the pull command to upload files onto the version control server
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
#include "pull.h"
#include "../network_client.h"  // for sendJsonRequest()
#include "../auth.h"            // for loadTokenAndUsername()
#include <thread>
#include <chrono>

#define PORT 9696

using json = nlohmann::json;
using namespace std;
using namespace std::filesystem;

const char* server_ip = "127.0.0.1"; 

/*
 *
 * TODO:-
 *  - Find the name of the file from the user and the server
 *
 * */
bool PullReq::recieveFileFromServer(const std::string& ip, int port, const std::string& username,
                              const std::string& repoName, const std::string& fileDestination, const std::string& fileName) {
    // Establishing a socket connection with the server for file transfer
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[client] ❌ Socket creation failed");
        return false;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // ✅ Convert IP once and check success
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        cerr << "[client] ❌ Invalid address or address not supported.\n";
        close(sock);
        return false;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // ✅ Now try to connect
    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "[client] ❌ Connection to server failed\n";
        close(sock);
        return false;
    }

    cout << "[client] ✅ Connected to server at " << server_ip << ":" << port << endl;

    // Recieving the filename and username for confimation from the server line by line
    auto readLine = [&](string& line) {
        char ch;
        line.clear();
        while (read(sock, &ch, 1) == 1 && ch != '\n') {
            line += ch;
        }
    };

    // Test varaibles to hold the information later used to cross validate
    string recieved_filename;
    string recieved_username;

    // Reading lines one by one 
    readLine(recieved_filename);
    readLine(recieved_username);
    cerr<<"Filename:"<<fileName<<"\t\t\tUsername:"<<username<<endl;

    // Confirming if the information given by the server matches with out database
    if(((fileName != recieved_filename) && (username != recieved_filename)))
    {
        cerr<<"Recieved filename:" << recieved_filename <<"\tRecieved Username:" <<recieved_username<<endl;
        cerr<<"Filename match:" << (fileName == recieved_filename) <<"\tUsername match:" << (username == recieved_username)<<endl;
        cerr<<"Verification failed"<<endl;
        return false;
    }

    // cerr<<"Filename:"<<fileName<<endl;
    // Creating a dublicate file to store the information from the server

    string dubFilePath = fileDestination + "___dublicate___" + fileName;
    cerr << "File Path: " << dubFilePath << endl;

    ofstream file(dubFilePath, ios::binary); // binary mode is important

    if (!file.is_open()) {
        cerr << "Failed to open file: " << dubFilePath << endl;
        perror("ofstream");
        return 1;
    }

    char buffer[4096];
    ssize_t bytesRead;
    
    while ((bytesRead = read(sock, buffer, sizeof(buffer))) > 0) {
        cerr << "Read " << bytesRead << " bytes\n";
        file.write(buffer, bytesRead);
        file.flush();
        if (!file) {
            cerr << "Write failed. Error: " << strerror(errno) << endl;
            return 1;
        }
    }
    file.flush();
    file.close();

    string dubFile = "/___dublicate___" + fileName;

    // Storing the orignal path of the file
    string filePath = fileDestination + fileName;


    // Confirm the file was created correctly
    if (std::filesystem::exists(dubFilePath)) {
        cerr << "✅ File created: " << dubFilePath << endl;
        cerr << "File size: " << std::filesystem::file_size(dubFilePath) << " bytes\n";
    } else {
        cerr << "❌ Duplicate file not found at expected path\n";
    }

    // Log paths
    cerr << "Original file path: " << filePath << endl;
    cerr << "Duplicate file path: " << dubFilePath << endl;

    // Remove old file
      // Try removing original if it exists
    if (remove(filePath) != 0) {
        std::cerr << "remove() failed: " << strerror(errno) << std::endl;
    } else {
        std::cerr << "Removed existing file: " << dubFilePath << std::endl;
    }

    shutdown(sock, SHUT_WR); 
    close(sock);
    
    return true;
}

/*
 * function name: getUsernameFromToken
 * Creation Date: 31st July, 2025
 * Author: Dev Rathod
 * 
 * This function excecutes the pullcommand and recieves the file from the server
 */
void PullReq::pullLatestCommand(const std::string& repoName, const std::string& folderPath, const std::string& filename) {
    // Storing the token and username for verification
    string token, username;
    auth.loadTokenAndUsername(token, username);

    cerr<<"Everything is fine before json"<<endl;

    // To store and validate the correct location of the file
    path fileLocation;

    cerr<<"In the pull cpp function"<<endl;
    // Finding out if the file storing path is a directory or a file
    if(is_regular_file(path(folderPath))){
        fileLocation = path(folderPath).parent_path();
    } else{
        fileLocation = path(folderPath);
    }
   
    cerr << "[client] Recieving file..\n";
    if (recieveFileFromServer(server_ip, PORT, username, repoName, fileLocation, filename)){
        cerr << "[client] Pull recieved successfully.\n";
    } else {
        cerr << "[client] Pull upload failed.\n";
    }

    cerr << "[client] Pulled latest commit. Saved as: " << filename << "\n";
}