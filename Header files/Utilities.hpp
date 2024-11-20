#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include "Bike.hpp"

using namespace std;
string getCurrentDate();
string generateSerialNumber();
vector<Bike> loadInventory(const string& filename);

#endif