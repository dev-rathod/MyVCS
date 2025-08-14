/*
 * Author: Dev Rathod
 * Email: dev-rathod@hotmail.com
 * Date: 31st July, 2025
 * 
 * This files performs the authentication of user using their token addr 
 * 
 * */
#ifndef COMMIT_H
#define COMMIT_H

#include <iostream>
#include <string>
#include <string>
#include "../auth.h"
#include "../network_client.h"  
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

class commit {
    public:
        bool sendFileToServer(const std::string& ip, int port, const std::string& username,
                        const std::string& repoName, const std::string& filePath);
        void commitCommand(const std::string& repoName, const std::string& filePath);
    private:
        Authentication auth;
        networkClient net;
};

#endif