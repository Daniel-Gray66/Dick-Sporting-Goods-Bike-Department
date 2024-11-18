#include "UserAuth.hpp"
#include <fstream>

UserAuth::UserAuth(const std::string& filename) : credentialsFile(filename) {
    loadCredentials();
}

void UserAuth::loadCredentials() {
    std::ifstream file(credentialsFile);
    std::string username, password;
    
    while (file >> username >> password) {
        credentials[username] = password;
    }
}

void UserAuth::saveCredentials() {
    std::ofstream file(credentialsFile);
    
    for (const auto& pair : credentials) {
        file << pair.first << " " << pair.second << "\n";
    }
}

bool UserAuth::registerUser(const std::string& username, const std::string& password) {
    if (credentials.find(username) != credentials.end()) {
        return false;
    }
    
    credentials[username] = password;
    saveCredentials();
    return true;
}

bool UserAuth::verifyUser(const std::string& username, const std::string& password) {
    auto it = credentials.find(username);
    return it != credentials.end() && it->second == password;
}