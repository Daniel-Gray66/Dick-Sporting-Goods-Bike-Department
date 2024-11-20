
#include "Menu.hpp"
#include <iostream>

using namespace std;

void Menu::displayMainMenu() {
    cout << "\n=== Dick's Sporting Goods Bike Department ===\n"
              << "1. Purchase a Bike\n"
              << "2. Pick Up Bike\n"
              << "3. Drop Off Bike for Assembly\n"
              << "4. Bike Service/Maintenance\n"
              << "5. Logout\n"
              << "Enter your choice: ";
}

void Menu::displayInventory(const vector<Bike>& inventory) {
    cout << "\n=== Available Bikes ===\n";
    for (size_t i = 0; i < inventory.size(); ++i) {
        cout << i + 1 << ". " << inventory[i].toString() << endl;
    }


}