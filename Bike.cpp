#include "Bike.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;


// Constructor is typically implemented in the header file since it's inline
    Bike::Bike(const string& make, const string& model, double price,
         const string& custName = "", const string& custPhone = "")
        : make(make), model(model), price(price), readyForPickup(true),
          customerName(custName), customerPhone(custPhone) {}


void Bike::setCustomer(const string& name, const string& phone) {
    customerName = name;
    customerPhone = phone;
}

bool Bike::isReadyForPickup() const {
    return readyForPickup;
}

void Bike::setReadyForPickup(bool ready) {
    readyForPickup = ready;
}

string Bike::getCustomerName() const {
    return customerName;
}

string Bike::getCustomerPhone() const {
    return customerPhone;
}

string Bike::getMake() const {
    return make;
}

string Bike::getModel() const {
    return model;
}

bool Bike::belongsToCustomer(const string& name, const string& phone) const {
    return customerName == name && customerPhone == phone;
}

// In your Bike class
string Bike::toString() const {
    ostringstream oss;
    oss << make << " " << model << " - $" << fixed << setprecision(2) << price;
    // Only add customer info if it's not "NA"
    if (customerName != "NA" && customerPhone != "NA") {
        oss << " (Customer: " << customerName << ", Phone: " << customerPhone << ")";
    }
    cout << endl;
    return oss.str();
}

double Bike::getPrice() const {
    return price;
}

void Bike::setPrice(double newPrice) {
    price = newPrice;
}