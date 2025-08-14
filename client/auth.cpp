#include <fstream>
#include "auth.h"
#include <nlohmann/json.hpp>

void Authentication::loadTokenAndUsername(std::string& token, std::string& username) {
    std::ifstream in("client_config.json");
    if (!in.is_open()) {
        throw std::runtime_error("❌ Failed to open client_config.json");
    }

    nlohmann::json config;
    try {
        in >> config;
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("❌ Failed to parse JSON: ") + e.what());
    }

    if (!config.contains("public_key") || !config.contains("username")) {
        throw std::runtime_error("❌ Config JSON missing required keys");
    }

    token = config["public_key"].get<std::string>();
    username = config["username"].get<std::string>();
}
