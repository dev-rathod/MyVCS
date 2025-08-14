#include <nlohmann/json.hpp>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sstream>
#include <iostream>
#include "network_client.h"

using json = nlohmann::json;

json networkClient::sendJsonRequest(const std::string& ip, int port, const json& request) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in servAddr{};
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &servAddr.sin_addr);

    cerr<<"NET: Connecting to sock"<<endl;
    if (connect(sock, (sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        std::cerr << "[network] Connection failed.\n";
        return {{"status", "error"}, {"message", "Connection failed"}};
    }

    cerr<<"Sending the string to the server"<<endl;
    std::string reqStr = request.dump() + "\n";
    cout<<"ReqStr:"<<reqStr<<endl;
    send(sock, reqStr.c_str(), reqStr.size(), 0);

    // char buffer[8192] = {0};
    // int bytesRead = read(sock, buffer, sizeof(buffer));
    // close(sock);

    // if (bytesRead <= 0) {
    //     std::cerr << "[network] ❌ Empty or no response from server.\n";
    //     return {{"status", "error"}, {"message", "Empty response from server"}};
    // }

    // std::string responseStr(buffer, bytesRead);
    // std::cerr << "Raw server response: " << responseStr << std::endl;

    // try {
    //     return json::parse(responseStr);
    // } catch (const json::parse_error& e) {
    //     std::cerr << "[network] JSON parse error: " << e.what() << std::endl;
    //     return {{"status", "error"}, {"message", "Invalid JSON response from server"}};
    // }
    std::string responseStr;
    char buffer[1024];
    int bytesRead;

    while ((bytesRead = read(sock, buffer, sizeof(buffer))) > 0) {
        responseStr.append(buffer, bytesRead);
        if (responseStr.back() == '\n') break; // crude way to detect JSON end
    }

    close(sock);

    try {
        return json::parse(responseStr);
    } catch (const std::exception& e) {
        std::cerr << "[network] JSON parsing error: " << e.what() << "\n";
        std::cerr << "[network] Response received: " << responseStr << "\n";
        return {{"status", "error"}, {"message", "Invalid JSON response"}};
    }

}
