/*
 * Author: Dev Rathod
 * Email: dev-rathod@hotmail.com
 * Date: 31st July, 2025
 * 
 * This files performs the authentication of user using their token addr 
 * 
 * */

#pragma once
#include <string>

class AuthManager {
public:
    bool isValidToken(const std::string& token);
    std::string getUsernameFromToken(const std::string& token);
};