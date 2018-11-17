//
// Created by orly on 11/3/18.
//
#include "../include/Table.h"
#include "../include/Dish.h"

//constructor
Table::Table(int t_capacity):
        capacity(t_capacity),
        open(false)
{}

//copy constructor
Table::Table(const Table &t):capacity(t.getCapacity()),open(t.open) {
    for(int i=0;i<t.orderList.size();i++){
        orderList.emplace_back(t.orderList[i].first,t.orderList[i].second);
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
        for(int i=0;i<t.orderList.size();i++){
            orderList.emplace_back(t.orderList[i].first,t.orderList[i].second);
        }
        customersList.clear();
        for (auto i:t.customersList) {
            customersList.push_back(i->clone());
        }
    }
    return (*this);
}

//destructor
Table::~Table() {
    for (int i=0;i<customersList.size();i++){
        delete(customersList[i]);
    }
}

//move constructor
Table::Table(Table &&other) noexcept:open(other.open),capacity(other.capacity),customersList(other.customersList){
    other.open= false;
    other.capacity=0;
    other.customersList.clear();
    orderList.reserve(other.orderList.size());
    for(int i=0;i<other.orderList.size();i++){
        orderList.emplace_back(other.orderList[i].first,other.orderList[i].second);
    }
    other.orderList.clear();
}


//move assignment operator
Table& Table::operator=(Table &&other) noexcept {
    if(this!=&other){
        customersList.clear();
        orderList.clear();
        open=other.open;
        capacity=other.capacity;
        customersList.reserve(other.customersList.size());
        customersList=other.customersList;
        orderList.reserve(other.orderList.size());
        for(int i=0;i<other.orderList.size();i++){
            orderList.emplace_back(other.orderList[i].first,other.orderList[i].second);
        }
    }
    return *this;
}


Table* Table::clone() { return new Table(*this);}

void Table::addCustomer(Customer* customer) {
    customersList.push_back(customer);
}

int Table::getCapacity() const { return capacity;}

//void Table::setorderspush(OrderPair orderPair) {orderList.push_back(orderPair);}///****

//void Table::setorderserase(int i) {orderList.erase(orderList.begin()+i);} ///*****

Customer* Table::getCustomer(int id) {
    int i=0;
    while(i<customersList.size() & customersList[i]->getId()!=id )
        i++;
    if(customersList[i]->getId()==id )
        return customersList[i];
    else
        return nullptr;
}

void Table::removeCustomer(int id) {
    int i=0;
    while(i<customersList.size()& customersList[i]->getId()!=id )
        i++;
    if(customersList[i]->getId()==id )
        customersList.erase(customersList.begin()+i);
}

std::vector<Customer*>& Table::getCustomers() {return customersList;}

std::vector<OrderPair>& Table::getOrders() { return orderList;}

void Table::closeTable() {
   // for(auto i:customersList)
     //   delete(i);
     customersList.clear();
    open= false;
}

void Table::openTable() {
    open=true;
}

int Table::getBill() {
    int bill=0;
    for(int i=0;i<orderList.size();i++)
        bill+=orderList[i].second.getPrice();
    return bill;
}

bool Table::isOpen() {return open;}


void Table::order(const std::vector<Dish> &menu) {
    //int j=0;
    std::vector<int> dish;

    for(int i=0;i<customersList.size();i++)
    {
        dish=customersList[i]->order(menu);
        for(int j=0; j<dish.size(); j++) {
            std::pair<int, Dish> p(customersList[i]->getId(), menu[dish.at(j)]);
            orderList.push_back(p);
        }
       /*if(!dish.empty()) {
            /*while(j<menu.size()&dish[0]!=menu[j].getId())
                j++;
        }
         if(dish.size()==2){
            while(j<menu.size()&dish[1]!=menu[j].getId())
                j++;
            std::pair<int,Dish > p(customersList[i]->getId(),menu[j]);
            orderList.push_back(p);
        }*/
    }
}

