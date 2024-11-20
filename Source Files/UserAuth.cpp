#include "UserAuth.hpp"
#include <fstream>
#include <iostream>
#include <limits>


using namespace std;

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

bool UserAuth::isUsernameUnique(const string& username) {
    ifstream usernameFile("text_files/usernames.txt");
    string storedUsername;
    
    while (getline(usernameFile, storedUsername)) {
        if (username == storedUsername) {
            usernameFile.close();
            return false;
        }
    }
    
    usernameFile.close();
    return true;
}


bool UserAuth::validateCredentials(const string& username, const string& password) {
    ifstream usernameFile("text_files/usernames.txt");
    ifstream passwordFile("text_files/passwords.txt");
    string storedUsername, storedPassword;
    
    while (getline(usernameFile, storedUsername) && getline(passwordFile, storedPassword)) {
        if (username == storedUsername && password == storedPassword) {
            usernameFile.close();
            passwordFile.close();
            return true;
        }
    }
    
    usernameFile.close();
    passwordFile.close();
    return false;
}



void UserAuth::saveCredentials() {
    std::ofstream file(credentialsFile);
    if (file.is_open()) {
        for (const auto& pair : credentials) {
            file << pair.first << " " << pair.second << "\n";
        }
        file.close();
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

string UserAuth::loginMenu() {
    std::string username, password;
    bool loginSuccessful = false;
    
    while (!loginSuccessful) {
        cout << "\n=== Welcome ===\n";
        cout << "1. Login\n";
        cout << "2. Create Account\n";
        cout << "Enter your choice: ";
        
        string input;
        getline(cin, input);
        
        if (input == "1") {
            cout << "\n=== Login ===\n";
            cout << "Username: ";
            getline(cin, username);
            cout << "Password: ";
            getline(cin, password);
            
            if (validateCredentials(username, password)) {
                loginSuccessful = true;
            } else {
                cout << "Invalid credentials. Please try again.\n";
            }
        }
        else if (input == "2") {
            bool validUsername = false;
            while (!validUsername) {
                cout << "\n=== Create Account ===\n";
                cout << "Enter new username: ";
                getline(cin, username);
                
                if (isUsernameUnique(username)) {
                    validUsername = true;
                } else {
                    cout << "Username already exists. Please choose a different username.\n";
                }
            }
            
            cout << "Enter new password: ";
            getline(cin, password);
            
            ofstream usernameFile("text_files/usernames.txt", std::ios::app);
            ofstream passwordFile("text_files/passwords.txt", std::ios::app);
            
            usernameFile << username << "\n";
            passwordFile << password << "\n";
            
            usernameFile.close();
            passwordFile.close();
            
            cout << "Account created successfully!\n";
            loginSuccessful = true;
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
    
    cout << "Welcome, " << username << "!\n";
    return username;
}




