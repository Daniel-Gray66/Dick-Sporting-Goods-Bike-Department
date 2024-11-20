//libraries 
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <filesystem>

//header files
#include "UserAuth.hpp"
#include "Bike.hpp"
#include "Customer.hpp"
#include "Utilities.hpp"
#include "Menu.hpp"
#include "Service_handler.hpp"

using namespace std;
// how to compile
//g++ -o bikeShop.exe "Source Files/main.cpp" "Source Files/Bike.cpp" "Source Files/Customer.cpp" "Source Files/Menu.cpp" "Source Files/Service_handler.cpp" "Source Files/UserAuth.cpp" "Source Files/Utilities.cpp" -I"Header Files"


int main() {
    if (!filesystem::exists("receipts")) {
        filesystem::create_directory("receipts");
    }
    
    srand(time(0));  // Seed random number generator
    
    UserAuth userAuth;  // Create UserAuth object
    string username = userAuth.loginMenu();  // Call loginMenu through the UserAuth object
    vector<Bike> inventory = loadInventory("text_files/Inventory.txt");
    
    while (true) {
        Menu menu;  // Create Menu object
        menu.displayMainMenu();  // Call displayMainMenu through the object
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                handlePurchase(username, inventory, menu);  // Call directly, not through menu
                break;
                
            case 2:
                handlePickup(username);  // Call directly, not through menu
                break;
                
            case 3:
                handleBikeDropOff(username);  // Call directly, not through menu
                break;
                
            case 4:
                handleBikeDropOff(username);  // Call directly, not through menu
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