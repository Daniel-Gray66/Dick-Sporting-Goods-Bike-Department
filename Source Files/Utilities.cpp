#include "Utilities.hpp"
#include <fstream>
#include <iostream>
#include "Bike.hpp"
#include <vector>
using namespace std;    

string getCurrentDate() {
    time_t now = time(0);
    string date = ctime(&now);
    return date.substr(0, date.length() - 1); // Remove newline
}

string generateSerialNumber() {
    const string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string serial = "BK-";
    // Generate 8 random characters
    for(int i = 0; i < 8; i++) {
        serial += chars[rand() % chars.length()];
    }
    return serial;
}

vector<Bike> loadInventory(const string& filename) {
    std::ifstream file(filename);
    std::vector<Bike> inventory;

    if (!file.is_open()) {
        std::cout << "Error: Could not open " << filename << std::endl;
        return inventory;
    }

    std::string line;
    while (getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue;
        
        std::string make, model, customerName, customerPhone;
        double price;
        
        // Parse the comma-separated line
        size_t pos = 0;
        std::string token;
        
        // Get make
        pos = line.find(",");
        make = line.substr(0, pos);
        line.erase(0, pos + 1);
        
        // Get model
        pos = line.find(",");
        model = line.substr(0, pos);
        line.erase(0, pos + 1);
        
        // Get price
        pos = line.find(",");
        price = std::stod(line.substr(0, pos));
        line.erase(0, pos + 1);
        
        // Get customerName
        pos = line.find(",");
        customerName = line.substr(0, pos);
        line.erase(0, pos + 1);
        
        // Get customerPhone
        customerPhone = line;
        
        Bike bike(make, model, price, customerName, customerPhone);
        inventory.push_back(bike);
    }

  
    return inventory;
}