#ifndef USER_AUTH_HPP
#define USER_AUTH_HPP

#include <string>
#include <unordered_map>
#include <fstream>
using namespace std;
class UserAuth {
private:
    std::string credentialsFile;
    std::unordered_map<std::string, std::string> credentials;
    
    void loadCredentials();
    void saveCredentials();


public:
    UserAuth(const std::string& filename = "credentials.txt");
    bool registerUser(const std::string& username, const std::string& password);
    bool verifyUser(const std::string& username, const std::string& password);
    string loginMenu();
    bool validateCredentials(const std::string& username, const std::string& password);
    bool isUsernameUnique(const std::string& username);
};

#endif