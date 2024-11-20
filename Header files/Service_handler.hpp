#ifndef SERVICE_HANDLER_HPP
#define SERVICE_HANDLER_HPP

#include <string>
#include <vector>
#include "Menu.hpp"
#include "Bike.hpp"

     void handlePurchase(const std::string& username, std::vector<Bike>& inventory, Menu& menu);
     void handlePickup(const std::string& username);
     void handleBikeDropOff(const std::string& username);

#endif