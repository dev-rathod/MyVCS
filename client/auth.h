/*
 * Author: Dev Rathod
 * Email: dev-rathod@hotmail.com
 * Date: 31st July, 2025
 * 
 * This files performs the authentication of user using their token addr 
 * 
 * */
#ifndef AUTH_H
#define AUTH_H

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

class Authentication {
public:
    void loadTokenAndUsername(std::string& token, std::string& username);
};

#endif