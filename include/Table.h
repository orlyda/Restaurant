//
// Created by adi on 11/6/18.
//
#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);//constructor
    Table(const Table &newtable);//copy constructor
    Table& operator=(const Table &t); //copy assignment operator
    virtual ~Table();//destructor
    Table(Table&& other);//move constructor
    Table& operator=(Table&& other); //move assignment operator
    virtual Table* clone();
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    void setorderspush(OrderPair orderPair);//***
    void setorderserase(int i);//***
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif