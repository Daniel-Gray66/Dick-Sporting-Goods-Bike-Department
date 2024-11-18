#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "UserAuth.hpp"
#include "Bike.hpp"
#include "Customer.hpp"
#include <sstream>
#include <ctime>
#include <filesystem>

using namespace std;

// Function prototypes
void displayInventory(const std::vector<Bike>& inventory);
void handlePurchase(const std::string& username, std::vector<Bike>& inventory);
void handlePickup(const std::string& username);
void handleBikeDropOff(const std::string& username);
std::vector<Bike> loadInventory(const std::string& filename);
string getCurrentDate(); 
string generateSerialNumber();

// Load inventory from file

// Add to function prototypes at top
string loginMenu();
bool validateCredentials(const std::string& username, const std::string& password);

// Add these new functions
bool validateCredentials(const string& username, const string& password) {
    ifstream usernameFile("usernames.txt");
    ifstream passwordFile("passwords.txt");
    string storedUsername, storedPassword;
    
    while (getline(usernameFile, storedUsername) && getline(passwordFile, storedPassword)) {
        if (username == storedUsername && password == storedPassword) {
            return true;
        }
    }
    
    usernameFile.close();
    passwordFile.close();
    return false;
}

bool isUsernameUnique(const string& username) {
    ifstream usernameFile("usernames.txt");
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

string loginMenu() {
    std::string username, password;
    bool loginSuccessful = false;
    
    while (!loginSuccessful) {
        std::cout << "\n=== Welcome ===\n";
        std::cout << "1. Login\n";
        std::cout << "2. Create Account\n";
        std::cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;
        
        
        switch (choice) {
            case 1: {
                cout << "\n=== Login ===\n";
                cout << "Username: ";
                cin.ignore();
                getline(cin, username);
                cout << "Password: ";
                getline(cin, password);
                
                if (validateCredentials(username, password)) {
                    loginSuccessful = true;
                } else {
                    cout << "Invalid credentials. Please try again.\n";
                }
                break;
            }
            case 2: {
                bool validUsername = false;
                cin.ignore();
                while (!validUsername) {
                    cout << "\n=== Create Account ===\n";
                    cout << "Enter new username: ";
                    getline(cin,username);
                    
                    if (isUsernameUnique(username)) {
                        validUsername = true;
                    } else {
                        cout << "Username already exists. Please choose a different username.\n";
                    }
                }
                
                cout << "Enter new password: ";
                getline(cin,password);
                
                // Add new user to separate files
                ofstream usernameFile("usernames.txt", std::ios::app);
                ofstream passwordFile("passwords.txt", std::ios::app);
                
                usernameFile << username << "\n";
                passwordFile << password << "\n";
                
                usernameFile.close();
                passwordFile.close();
                
                cout << "Account created successfully!\n";
                loginSuccessful = true;
                break;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    
    cout << "Welcome, " << username << "!\n";
    return username;
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

    cout << "Just before returning inventory" << endl;
    for (const auto& bike : inventory) {
        cout << bike.toString() << endl;
    }  

    cout << "Inventory loaded successfully.\n";
    return inventory;
}

void displayMainMenu() {
    std::cout << "\n=== Dick's Sporting Goods Bike Department ===\n"
              << "1. Purchase a Bike\n"
              << "2. Pick Up Bike\n"
              << "3. Drop Off Bike for Assembly\n"
              << "4. Bike Service/Maintenance\n"
              << "5. Logout\n"
              << "Enter your choice: ";
}

void handlePurchase(const std::string& username, std::vector<Bike>& inventory) {
    cout << "\n=== Available Bikes ===\n";
    displayInventory(inventory);
    
    cout << "\nEnter the bike number you wish to purchase (0 to cancel): ";
    int choice;
    cin >> choice;
    
    if (choice == 0) {
        cout << "Purchase cancelled.\n";
        return;
    }
    
    if (choice > 0 && choice <= inventory.size()) {
        const Bike& selectedBike = inventory[choice - 1];
        std::cout << "You have selected: " << selectedBike.toString() << std::endl;
        
        // Calculate total with tax (assuming 8% sales tax)
        double subtotal = selectedBike.getPrice();
        double tax = subtotal * 0.08;
        double total = subtotal + tax;
        
        std::cout << "Subtotal: $" << std::fixed << std::setprecision(2) << subtotal << std::endl;
        std::cout << "Tax (8%): $" << tax << std::endl;
        std::cout << "Total: $" << total << std::endl;
        
        char confirm;
        std::cout << "Confirm purchase? (y/n): ";
        std::cin >> confirm;
        
        if (tolower(confirm) == 'y') {
            // Remove bike from inventory vector
            inventory.erase(inventory.begin() + choice - 1);
            
            // Update inventory.txt file
            ofstream outFile("inventory.txt");
            for (const auto& bike : inventory) {
                outFile << bike.getMake() << ","
                       << bike.getModel() << ","
                       << bike.getPrice() << ","
                       << bike.getCustomerName() << ","
                       << bike.getCustomerPhone() << "\n";
            }
            outFile.close();
            
            cout << "Purchase successful! Come and pick up your bike soon.\n";
        }
    } else {
        cout << "Invalid bike number. Please try again.\n";
    }
}

void handlePickup(const string& username) {
    cout << "\n=== Bike Pickup ===\n";
    
    // Read from assembly orders file
    ifstream orderFile("assembly_orders.txt");
    vector<string> allOrders;
    bool foundOrder = false;
    string line;
    vector<string> userOrders;
    
    cout << "Orders for " << username << ":\n";
    while (getline(orderFile, line)) {
        // Parse the comma-separated line (username,make,model,serial)
        size_t pos = line.find(",");
        string orderUsername = line.substr(0, pos);
        
        if (orderUsername == username) {
            foundOrder = true;
            string bikeDetails = line;
            userOrders.push_back(bikeDetails);
            
            // Parse and display bike details
            string remainingDetails = line.substr(pos + 1);
            pos = remainingDetails.find(",");
            string make = remainingDetails.substr(0, pos);
            remainingDetails = remainingDetails.substr(pos + 1);
            pos = remainingDetails.find(",");
            string model = remainingDetails.substr(0, pos);
            string serial = remainingDetails.substr(pos + 1);
            
            cout << userOrders.size() << ". " << make << " " << model << "\n";
            cout << "   Serial: " << serial << "\n";
        }
        allOrders.push_back(line);
    }
    orderFile.close();

    if (!foundOrder) {
        cout << "No bikes currently available for pickup.\n";
        return;
    }

    // Ask for serial number verification
    cout << "\nEnter the serial number of the bike you wish to pick up: ";
    string inputSerial;
    cin >> inputSerial;

    // Find and verify the bike with matching serial
    bool serialFound = false;
    string selectedOrder;
    for (const string& order : userOrders) {
        if (order.find(inputSerial) != string::npos) {
            serialFound = true;
            selectedOrder = order;
            break;
        }
    }

    if (!serialFound) {
        cout << "Invalid serial number. Pickup cancelled.\n";
        return;
    }


    // Write remaining orders back to file
    ofstream outFile("Pickup.txt");
    for (const string& order : allOrders) {
        outFile << order << "\n";
    }
    outFile.close();

    // Parse and confirm pickup details
    size_t pos = selectedOrder.find(",");
    string details = selectedOrder.substr(pos + 1);
    pos = details.find(",");
    string make = details.substr(0, pos);
    details = details.substr(pos + 1);
    pos = details.find(",");
    string model = details.substr(0, pos);
    string serial = details.substr(pos + 1);
    
    cout << "\nPickup confirmed for:\n";
    cout << "Make: " << make << "\n";
    cout << "Model: " << model << "\n";
    cout << "Serial: " << serial << "\n";
    cout << "Thank you for choosing our service!\n";

    // After successful pickup, save pickup confirmation
    string pickupFileName = "receipts/" + username + "_" + serial + "_pickup.txt";
    ofstream pickupFile(pickupFileName);
    pickupFile << "=== BIKE PICKUP CONFIRMATION ===\n\n"
               << "Customer: " << username << "\n"
               << "Date: " << getCurrentDate() << "\n\n"
               << "BIKE DETAILS:\n"
               << "Make: " << make << "\n"
               << "Model: " << model << "\n"
               << "Serial: " << serial << "\n\n"
               << "Pickup completed successfully.\n"
               << "Thank you for choosing our service!\n";
    pickupFile.close();

    cout << "\nPickup confirmed for:\n";
    cout << "Make: " << make << "\n";
    cout << "Model: " << model << "\n";
    cout << "Serial: " << serial << "\n";
    cout << "A pickup receipt has been saved to: " << pickupFileName << "\n";
    cout << "Thank you for choosing our service!\n";
}

void handleBikeDropOff(const string& username) {
    cout << "\n=== Bike Drop-off Service ===\n";
    cout << "1. Bike Assembly ($49.99)\n";
    cout << "2. Basic Tune-Up ($29.99)\n";
    cout << "Enter your choice (1-2): ";
    
    int choice;
    cin >> choice;
    
    // Service type and cost
    string serviceType;
    double serviceCost;
    if (choice == 1) {
        serviceType = "Assembly";
        serviceCost = 49.99;
    } else if (choice == 2) {
        serviceType = "Basic Tune-Up";
        serviceCost = 29.99;
    } else {
        cout << "Invalid choice. Operation cancelled.\n";
        return;
    }
    
    // Get bike details
    string make, model;
    cout << "\nPlease provide bike details:\n";
    cout << "Make: ";
    cin.ignore();
    getline(cin, make);
    cout << "Model: ";
    getline(cin, model);
    
    // Generate unique serial number
    string serialNumber = generateSerialNumber();
    
    // Save order to tracking file
    string filename = (serviceType == "Assembly") ? "assembly_orders.txt" : "service_orders.txt";
    ofstream orderFile(filename, ios::app);
    orderFile << username << "," << make << "," << model << "," << serialNumber << "," << serviceType << "\n";
    orderFile.close();
    
    // Save receipt
    string receiptFileName = "receipts/" + username + "_" + serialNumber + "_" + serviceType + ".txt";
    ofstream receiptFile(receiptFileName);
    receiptFile << "=== BIKE " << serviceType << " ORDER CONFIRMATION ===\n\n"
                << "Customer: " << username << "\n"
                << "Date: " << getCurrentDate() << "\n\n"
                << "BIKE DETAILS:\n"
                << "Make: " << make << "\n"
                << "Model: " << model << "\n"
                << "Serial Number: " << serialNumber << "\n\n"
                << "Service Type: " << serviceType << "\n"
                << "Service Cost: $" << fixed << setprecision(2) << serviceCost << "\n"
                << "IMPORTANT: Please save this serial number for pickup!\n"
                << "Estimated completion time: 2-3 business days\n";
    receiptFile.close();
    
    // Display confirmation
    cout << "\nBike has been registered for " << serviceType << ".\n";
    cout << "Bike Details:\n";
    cout << "Customer: " << username << "\n";
    cout << "Make: " << make << "\n";
    cout << "Model: " << model << "\n";
    cout << "Serial Number: " << serialNumber << "\n";
    cout << "Service Type: " << serviceType << "\n";
    cout << "Service Cost: $" << serviceCost << "\n";
    cout << "A receipt has been saved to: " << receiptFileName << "\n";
    cout << "Estimated completion time: 2-3 business days\n";
}

void displayInventory(const std::vector<Bike>& inventory) {
    int index = 1;
    for (const auto& bike : inventory) {
        cout << index++ << ". " << bike.toString() << endl;
    }
}

// Add this helper function to generate a random serial number
string generateSerialNumber() {
    const string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string serial = "BK-";
    // Generate 8 random characters
    for(int i = 0; i < 8; i++) {
        serial += chars[rand() % chars.length()];
    }
    return serial;
}

// Helper function to get current date as string
string getCurrentDate() {
    time_t now = time(0);
    string date = ctime(&now);
    return date.substr(0, date.length() - 1); // Remove newline
}

int main() {
    if (!filesystem::exists("receipts")) {
        filesystem::create_directory("receipts");
    }
    
    string username = loginMenu();
    vector<Bike> inventory = loadInventory("Inventory.txt");
    
    while (true) {
        displayMainMenu();
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                handlePurchase(username, inventory);
                break;
                
            case 2:
                handlePickup(username);
                break;
                
            case 3:
                handleBikeDropOff(username);
                break;
                
            case 4:
                handleBikeDropOff(username);
                break;
                
            case 5:
                cout << "Thank you for visiting! Goodbye!\n";
                return 0;
                
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    
    return 0;
}