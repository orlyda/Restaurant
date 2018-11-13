//
// Created by adi on 11/6/18.
//

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"
using namespace std;

class Customer{
public:
    Customer(std::string c_name, int c_id);//constructor
    Customer(Customer &c);//copy constructor
    virtual Customer* clone()=0;
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    bool alreadyOrdered();
    void setOrder();
    const string getmyType();///***********
    void setAllMyOrders(int newone);//****
    std::vector<int> getAllMyOrders();//*****
protected:
    string myType;////*****
    vector<int> allMyOrders;///***
private:
    const std::string name;
    const int id;
    bool alreadyOrder;
};


class VegetarianCustomer : public Customer {
public:
	VegetarianCustomer(std::string name, int id);
	VegetarianCustomer* clone();
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
	CheapCustomer* clone();
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
private:
};


class SpicyCustomer : public Customer {
public:
	SpicyCustomer(std::string name, int id);
	SpicyCustomer* clone();
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
private:
};


class AlchoholicCustomer : public Customer {
public:
	AlchoholicCustomer(std::string name, int id);
	AlchoholicCustomer* clone();
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
private:
    int alcPrice;//show the price of the most expensive ALC the customer ordered until now
    int alcId;//show the price of the most expensive ALC the customer ordered until now
};


#endif