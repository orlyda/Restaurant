//
// Created by orly on 11/3/18.
//
#include "../include/Table.h"
#include "../include/Dish.h"

//constructor
Table::Table(int t_capacity):
        capacity(t_capacity),
        open(false), customersList(),orderList()
{}

//copy constructor
Table::Table(const Table &t):capacity(t.getCapacity()),open(t.open),customersList(),orderList(){
    for (const auto &i : t.orderList) {
        orderList.emplace_back(i.first, i.second);
    }
    for(auto i:t.customersList) {
        customersList.push_back(i->clone());
    }
}

//copy assignment operator
Table& Table::operator=(const Table& t) {
    if(this!=&t) {
        capacity = t.getCapacity();
        open = t.open;
        orderList.clear();
        orderList.reserve(t.orderList.size());
        for (const auto &i : t.orderList) {
            orderList.emplace_back(i.first, i.second);
        }
        for (auto j:customersList)
            delete j;
        for (auto i:t.customersList) {
            customersList.push_back(i->clone());
        }
    }
    return (*this);
}

//destructor
Table::~Table() {
    for (auto &i : customersList) {
        delete i;
        i= nullptr;
    }
    //orderList.clear();
}

//move constructor
Table::Table(Table &&other) noexcept:
capacity(other.capacity),open(other.open),customersList(other.customersList), orderList(){
    other.open= false;
    other.capacity=0;
    for(auto j:other.customersList)
        delete(j);
    orderList.reserve(other.orderList.size());
    for (auto &i : other.orderList) {
        orderList.emplace_back(i.first, i.second);
    }
    other.orderList.clear();
}

//move assignment operator
Table& Table::operator=(Table &&other) noexcept {
    if(this!=&other){
        for(auto j:customersList)
            delete(j);
        orderList.clear();
        open=other.open;
        capacity=other.capacity;
        customersList.reserve(other.customersList.size());
        customersList=other.customersList;
        orderList.reserve(other.orderList.size());
        for (auto &i : other.orderList) {
            orderList.emplace_back(i.first, i.second);
        }
    }
    return *this;
}


Table* Table::clone() { return new Table(*this);}

void Table::addCustomer(Customer* customer) {
    customersList.push_back(customer);
}

int Table::getCapacity() const { return capacity;}

Customer* Table::getCustomer(int id) {
    int i=0;
    while((unsigned)i<customersList.size() && customersList[i]->getId()!=id )
        i++;
    if(customersList[i]->getId()==id )
        return customersList[i];
    else
        return nullptr;
}

void Table::removeCustomer(int id) {
    int i=0;
    while((unsigned)i<customersList.size()&& customersList[i]->getId()!=id )
        i++;
    if(customersList[i]->getId()==id ) {
        delete customersList[i];
        customersList[i]= nullptr;
    }
}

std::vector<Customer*>& Table::getCustomers() {return customersList;}

std::vector<OrderPair>& Table::getOrders() { return orderList;}

void Table::closeTable() {
    for(auto i:customersList)
        delete(i);
    customersList.clear();
    orderList.clear();
    open= false;
}

void Table::openTable() {
    open=true;
}

int Table::getBill() {
    int bill=0;
    for (auto &i : orderList)
        bill+= i.second.getPrice();
    return bill;
}

bool Table::isOpen() {return open;}


void Table::order(const std::vector<Dish> &menu) {
    std::vector<int> dish;
    for (auto &i : customersList) {
        dish= i->order(menu);
        for (int j : dish) {
            std::pair<int, Dish> p(i->getId(), menu[j]);
            orderList.push_back(p);
        }
    }
}

