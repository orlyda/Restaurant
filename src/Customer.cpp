//
// Created by orly on 11/3/18.
//
#include <Customer.h>

#include "../include/Customer.h"

//constructor
Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id),alreadyOrder(false){}

//copy constructor
Customer::Customer(Customer &c): Customer(c.getName(),c.getId()) {}

int Customer::getId() const { return id;}

std::string Customer::getName() const { return name;}

bool Customer::alreadyOrdered() { return alreadyOrder;}

void Customer::setOrder() {alreadyOrder=true;}

const string Customer::getmyType() { return myType;} ///***

//std::vector<int> Customer::getAllMyOrders() { return allMyOrders;} //***

//void Customer::setAllMyOrders(int newone) {allMyOrders.push_back(newone);}///***

CheapCustomer* CheapCustomer::clone() { return new CheapCustomer(*this);}

AlchoholicCustomer* AlchoholicCustomer::clone() { return new AlchoholicCustomer(*this);}

VegetarianCustomer* VegetarianCustomer::clone() { return new VegetarianCustomer(*this);}

SpicyCustomer* SpicyCustomer::clone() { return new SpicyCustomer(*this);}

CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id){ //added
    myType="chp";
}

std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    if(!alreadyOrdered()) {
        int min = 100000000; //the price of the cheapest dish
        int minid =-1; //the id of the cheapest dish
        vector<int> v;
        for (int i = 0; i < menu.size(); i++) {
            if (menu[i].getType() != ALC & menu[i].getType() != BVG &
            (menu[i].getPrice() < min |(menu[i].getPrice() == min & menu[i].getId()<minid)))  {
                min = menu[i].getPrice();
                minid = menu[i].getId();
            }
        }
        if(min!=100000000) {
            setOrder();
            //setAllMyOrders(minid);///***
            v.push_back(minid);
            return v;
        }
    }
    return std::vector<int>() ;
}

std::string CheapCustomer::toString() const {
    return std::to_string(this->getId())+" "+this->getName();
}

std::string SpicyCustomer::toString() const {}


SpicyCustomer::SpicyCustomer(std::string name, int id): Customer(name,id){//added
    myType="spc";
}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    int max=0;
    int min=100000000;
    int id=-1;//the id of the chosen dish
    vector<int> v;
    if(!alreadyOrdered()) {
        for (int i = 0; i < menu.size(); i++) {
            if (menu[i].getType()==SPC & (menu[i].getPrice() > max |(menu[i].getPrice() ==max & menu[i].getId()<id)) ) {
                max = menu[i].getPrice();
                id = menu[i].getId();
            }
        }
        setOrder();
        if(id!=-1){
            //setAllMyOrders(id);///***
            v.push_back(id);
            return v;
        }
        return std::vector<int>() ;
    }
    else{
        for (int i = 0; i < menu.size(); i++) {
            if (menu[i].getType()==BVG &(menu[i].getPrice() < min |(menu[i].getPrice() ==min &menu[i].getId()<id)) ) {
                min = menu[i].getPrice();
                id = menu[i].getId();
            }
        }
        if(id!=-1){
           // setAllMyOrders(id);///***
           v.push_back(id);
            return v;
        }
        return std::vector<int>() ;
    }
}

VegetarianCustomer::VegetarianCustomer(std::string name, int id):Customer(name,id) { //added
    myType="veg";
}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
        int max = 0; //the price of the most expensive BVG
        int minid =100000000; //the id of the veg dish with the smallest id
        int maxid=-1; //the id of the most expensive BVG
        std::vector<int> v;
        for (int i = 0; i < menu.size(); i++) {
            if (menu[i].getId()<minid & menu[i].getType() == VEG) {
                minid = menu[i].getId();
            }
            if(menu[i].getType()==BVG &(menu[i].getPrice()>max||(menu[i].getPrice()==max&menu[i].getId()<maxid))) {
                maxid=menu[i].getId();
                max=menu[i].getPrice();
            }
        }
        if(minid!=100000000) {
           // setAllMyOrders(minid);///***
            v.push_back(minid);
        }
        if(maxid>-1)
           // setAllMyOrders(maxid);///***
            v.push_back(maxid);
        return v ;
}

std::string VegetarianCustomer::toString() const {
    return std::to_string(this->getId())+" "+this->getName();
}

AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) :Customer(name,id),alcPrice(-1),alcId(-1){
    myType="alc";
}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    int id=-1;
    int min=10000000;
    vector<int > v;
    for (int i=0;i<menu.size();i++){
        if (menu[i].getType() == ALC & (menu[i].getPrice()>alcPrice | (menu[i].getPrice()==alcPrice&menu[i].getId()>alcId))
        &(menu[i].getPrice() < min |(menu[i].getPrice() == min & menu[i].getId()<id)))  {
            id=menu[i].getId();
            min=menu[i].getPrice();
        }
    }
    if(id!=-1){
        alcId=id;
        alcPrice=min;
        //setAllMyOrders(id);///***
        v.push_back(id);
        return v;
    }
    return std::vector<int>();

}

std::string AlchoholicCustomer::toString() const {
    return std::to_string(this->getId())+" "+this->getName();
}
