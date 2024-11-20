#include "Service_handler.hpp"
#include "Menu.hpp"
#include <iostream> 
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include "Bike.hpp"
#include "Utilities.hpp"
#include <filesystem>

using namespace std;

 
void handlePurchase(const std::string& username, std::vector<Bike>& inventory, Menu& menu) {
    cout << "\n=== Available Bikes ===\n";
    menu.displayInventory(inventory);
    
    cout << "\nEnter the bike number you wish to purchase (0 to cancel): ";
    int choice;
    cin >> choice;
    
    if (choice == 0) {
        cout << "Purchase cancelled.\n";
        return;
    }
    
    if (choice > 0 && choice <= inventory.size()) {
        const Bike& selectedBike = inventory[choice - 1];
        cout << "You have selected: " << selectedBike.toString() << std::endl;
        
        // Calculate total with tax (assuming 8% sales tax)
        double subtotal = selectedBike.getPrice();
        double tax = subtotal * 0.08;
        double total = subtotal + tax;
        
        cout << "Subtotal: $" << fixed << setprecision(2) << subtotal << endl;
        cout << "Tax (8%): $" << tax << endl;
        cout << "Total: $" << total << endl;
        
        char confirm;
        cout << "Confirm purchase? (y/n): ";
        cin >> confirm;
        
        if (tolower(confirm) == 'y') {
            string serialNumber = to_string(time(0));
            string serviceType = "PURCHASE";
            
            // Print current directory and attempted file path
            cout << "Debug - Current Directory: " << std::filesystem::current_path().string() << "\n";
            
            // Try the exact same path as your working service receipts
            string receiptFileName = "text_files/receipts/" + username + "_" + serialNumber + "_" + serviceType + ".txt";
            cout << "Debug - Attempting to create file at: " << receiptFileName << "\n";
            
            ofstream receiptFile(receiptFileName);
            receiptFile << "=== BIKE PURCHASE RECEIPT ===\n\n"
                       << "Customer: " << username << "\n"
                       << "Date: " << getCurrentDate() << "\n\n"
                       << "BIKE DETAILS:\n"
                       << "Make: " << selectedBike.getMake() << "\n"
                       << "Model: " << selectedBike.getModel() << "\n"
                       << "Serial Number: " << serialNumber << "\n\n"
                       << "PAYMENT DETAILS:\n"
                       << "Subtotal: $" << fixed << setprecision(2) << subtotal << "\n"
                       << "Tax (8%): $" << tax << "\n"
                       << "Total: $" << total << "\n\n"
                       << "IMPORTANT: Please save this receipt for your records!\n";
            receiptFile.close();

            // Remove bike from inventory vector
            inventory.erase(inventory.begin() + choice - 1);
            
        

            // Update inventory.txt file
            ofstream outFile("text_files/inventory.txt");
            for (const auto& bike : inventory) {
                outFile << bike.getMake() << ","
                       << bike.getModel() << ","
                       << bike.getPrice() << ","
                       << bike.getCustomerName() << ","
                       << bike.getCustomerPhone() << "\n";
            }
            outFile.close();
        }
    } else {
        cout << "Invalid bike number. Please try again.\n";
    }
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
    string filename = (serviceType == "Assembly") ? "text_files/assembly_orders.txt" : "text_files/service_orders.txt";
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

void handlePickup(const string& username) {
    cout << "\n=== Bike Pickup ===\n";
    
    // Read from both assembly and service orders files
    vector<string> allOrders;
    bool foundOrder = false;
    vector<string> userOrders;
    
    // Helper function to read orders from a file
    auto readOrdersFile = [&](const string& filename) {
        ifstream orderFile(filename);
        string line;
        
        while (getline(orderFile, line)) {
            size_t pos = line.find(",");
            string orderUsername = line.substr(0, pos);
            
            if (orderUsername == username) {
                foundOrder = true;
                userOrders.push_back(line);
                
                // Parse and display bike details
                string remainingDetails = line.substr(pos + 1);
                pos = remainingDetails.find(",");
                string make = remainingDetails.substr(0, pos);
                remainingDetails = remainingDetails.substr(pos + 1);
                pos = remainingDetails.find(",");
                string model = remainingDetails.substr(0, pos);
                remainingDetails = remainingDetails.substr(pos + 1);
                pos = remainingDetails.find(",");
                string serial = remainingDetails.substr(0, pos);
                string serviceType = remainingDetails.substr(pos + 1);
                
                cout << userOrders.size() << ". " << make << " " << model << " (" << serviceType << ")\n";
                cout << "   Serial: " << serial << "\n";
            }
            allOrders.push_back(line);
        }
        orderFile.close();
    };

    cout << "Orders for " << username << ":\n";
    readOrdersFile("text_files/assembly_orders.txt");
    readOrdersFile("text_files/service_orders.txt");

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

    // Update both files to remove the picked-up order
    ofstream assemblyFile("text_files/assembly_orders.txt");
    ofstream serviceFile("text_files/service_orders.txt");
    
    for (const string& order : allOrders) {
        if (order != selectedOrder) {
            if (order.find("Assembly") != string::npos) {
                assemblyFile << order << "\n";
            } else {
                serviceFile << order << "\n";
            }
        }
    }
    
    assemblyFile.close();
    serviceFile.close();

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