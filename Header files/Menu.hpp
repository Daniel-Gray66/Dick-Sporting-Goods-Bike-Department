#ifndef MENU_HPP
#define MENU_HPP
#include <string>


#include <vector>
#include "Bike.hpp"

using namespace std;

class Menu {                                                
public:
    void displayMainMenu();
    void displayInventory(const vector<Bike>& inventory);
};

#endif