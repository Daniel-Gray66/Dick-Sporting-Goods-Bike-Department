#include "Customer.hpp"
#include <sstream>

Customer::Customer(string username, string name, string phoneNumber)
    : username(username), name(name), phoneNumber(phoneNumber) {
    updateLastVisit();
}

void Customer::updateLastVisit() {
    lastVisit = time(nullptr);
}

string Customer::toString() const {
    stringstream ss;
    ss << "Customer: " << name << " (Phone: " << phoneNumber << ")";
    return ss.str();
}

 string Customer::getUsername() const { return username; }
    string Customer::getName() const { return name; }
    string Customer::getPhoneNumber() const { return phoneNumber; }
    time_t Customer:: getLastVisit() const { return lastVisit; }