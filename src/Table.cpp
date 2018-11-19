//
// Created by orly on 11/3/18.
//
#include "../include/Table.h"
#include "../include/Dish.h"

//constructor
Table::Table(int t_capacity):
        capacity(t_capacity), open(false), customersList(),orderList()
{}

//copy constructor
Table::Table(const Table &t):capacity(t.capacity),open(t.open),customersList(),orderList(){
    for (auto &i : t.orderList) {
        orderList.emplace_back(i.first, i.second);
    }
    for(auto &i:t.customersList) {
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
        for (auto & j:customersList)
            if(j!= nullptr) {
                delete j;
                j = nullptr;
            }
        customersList.clear();

        for (auto & i:t.customersList) {
            customersList.push_back(i->clone());
        }
    }
    return (*this);
}

//destructor
Table::~Table() {
    for (auto &i : customersList) {
        if(i!= nullptr) {
            delete i;
            i = nullptr;
        }
    }
    customersList.clear();
    orderList.clear();
}

//move constructor
Table::Table(Table &&other) noexcept:
capacity(other.capacity),open(other.open),customersList(), orderList(){
    other.open= false;
    other.capacity=0;

    for(auto j:customersList) {
        if(j!= nullptr) {
            delete (j);
            j= nullptr;
        }
    }
    customersList.clear();
    for(auto & j:other.customersList){
        customersList.push_back(j->clone());
    }
    for(auto & j:other.customersList) {
        if(j!= nullptr) {
            delete (j);
            j= nullptr;
        }
    }

    orderList.reserve(other.orderList.size());
    for (auto &i : other.orderList) {
       orderList.emplace_back(i.first, i.second);
    }
    other.orderList.clear();
}

//move assignment operator
Table& Table::operator=(Table &&other) noexcept {
    if(this!=&other){
        for(auto &j:customersList) {
            if(j!= nullptr) {
                delete (j);
                j= nullptr;
            }
        }
        customersList.clear();
        orderList.clear();
        open=other.open;
        capacity=other.capacity;
        customersList.resize(other.customersList.size()); //resize instead reserve
      //  customersList=other.customersList;
        for(auto & c:other.customersList){
            customersList.push_back(c->clone());
        }
        orderList.reserve(other.orderList.size());
        for (auto &i : other.orderList) {
            int id = i.first;
            Dish dish = i.second;
            OrderPair pair(id,dish);
            orderList.push_back(pair);
         //   orderList.emplace_back(i.first, i.second);
        }
    }
    return *this;
}


Table* Table::clone() { return new Table(*this);}

void Table::addCustomer(Customer* customer) {
    customersList.push_back(customer->clone());
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
    //for(auto i:customersList)
     //   delete(i);
    //customersList.clear();
    //orderList.clear();
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

