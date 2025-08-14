/*
 * Author: Dev Rathod
 * Email: dev-rathod@hotmail.com
 * Date: 31st July, 2025
 * 
 * This files performs the authentication of user using their token addr 
 * 
 * */
#ifndef NET_H
#define NET_H

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

class networkClient {
    public:
        json sendJsonRequest(const std::string& ip, int port, const json& request);
};

#endif