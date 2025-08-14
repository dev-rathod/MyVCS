/*
 * Author: Dev Rathod
 * Email: dev-rathod@hotmail.com
 * Date: 31st July, 2025
 * 
 * This files performs the authentication of user using their token addr 
 * 
 * */
#ifndef PULL_H
#define PULL_H

#include <iostream>
#include <string>
#include "../network_client.h"
#include "../auth.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

class PullReq {
    public:
        void pullLatestCommand(const std::string& repoName, const std::string& folderPath, const std::string& filename);
        bool recieveFileFromServer(const std::string& ip, int port, const std::string& username,
                              const std::string& repoName, const std::string& fileDestination, const std::string& fileName);
    private:
        Authentication auth;
        networkClient net;
};

#endif