#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <string>
#include <ctime>
using namespace std;

class Customer {
private:
    std::string username;
    std::string name;
    std::string phoneNumber;
    time_t lastVisit;

public:
    Customer(string username, string name, string phoneNumber);
    
    // Getters
    string getUsername() const;
    string getName() const ;
    string getPhoneNumber() const;
    time_t getLastVisit() const;
    
    // Update last visit time
    void updateLastVisit();
    
    // Display customer information
    std::string toString() const;
};

#endif