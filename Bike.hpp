#ifndef BIKE_HPP
#define BIKE_HPP
#include <string>
using namespace std;
class Bike {
private:
    std::string make;
    std::string model;
    bool readyForPickup;
    // Customer information
    std::string customerName;
    std::string customerPhone;
    double price;

public:
    // Constructor
    Bike(const string& , const string& , double price,
         const string& , const string& custPhone);
    
    // Getters and setters
    void setCustomer(const string& , const string& );
    // ... existing code ...
    // ... other methods ...
    string toString() const;  // Add this declaration
    
    bool isReadyForPickup() const ;
    void setReadyForPickup(bool);
    
    string getCustomerName() const ;
    string getCustomerPhone() const ;
    string getMake() const;
    string getModel() const;
    
    bool belongsToCustomer(const string& name, const string& phone) const;
    double getPrice() const;
    void setPrice(double);
};

#endif
