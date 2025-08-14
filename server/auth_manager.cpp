/*
 * Author: Dev Rathod
 * Email: devmanojrathod@gmail.com
 * Date: 31st July, 2025
 * 
 * This files performs the authentication of user using their token addr 
 * 
 * */
// auth_manager.cpp
#include "auth_manager.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/*
 * function name: isValidToken
 * Creation Date: 31st July, 2025
 * Author: Dev Rathod
 * 
 * Checks if the user's token is valid and is present in the user_db database
 */
bool AuthManager::isValidToken(const std::string& token) {
    std::ifstream file("server/user_db.json");
    if (!file.is_open()) return false;

    json db;
    file >> db;

    for (auto& [username, userInfo] : db["users"].items()) {
        if (userInfo["public_key"] == token) {
            return true;
        }
    }

    return false;
}

/*
 * function name: getUsernameFromToken
 * Creation Date: 31st July, 2025
 * Author: Dev Rathod
 * 
 * Generates the user from the database using the public key address
 */
std::string AuthManager::getUsernameFromToken(const std::string& token) {
    std::ifstream file("server/user_db.json");
    if (!file.is_open()) return "";

    json db;
    file >> db;

    for (auto& [username, userInfo] : db["users"].items()) {
        if (userInfo["public_key"] == token) {
            return username;
        }
    }

    return "";
}