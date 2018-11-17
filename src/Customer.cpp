//
// Created by orly on 11/3/18.
//
#include <Customer.h>

#include "../include/Customer.h"
#include <iostream>
using namespace std;

//constructor
Customer::Customer(std::string c_name, int c_id):name(std::move(c_name)),id(c_id),alreadyOrder(false){}

//copy constructor
Customer::Customer(Customer &c): Customer(c.getName(),c.getId()) {}

int Customer::getId() const { return id;}

std::string Customer::getName() const { return name;}

bool Customer::alreadyOrdered() { return alreadyOrder;}

void Customer::setOrder() {alreadyOrder=true;}

const string Customer::getmyType() { return myType;} ///***

CheapCustomer* CheapCustomer::clone() { return new CheapCustomer(*this);}

AlchoholicCustomer* AlchoholicCustomer::clone() { return new AlchoholicCustomer(*this);}

VegetarianCustomer* VegetarianCustomer::clone() { return new VegetarianCustomer(*this);}

SpicyCustomer* SpicyCustomer::clone() { return new SpicyCustomer(*this);}

CheapCustomer::CheapCustomer(std::string name, int id):Customer(std::move(name),id){ //added
    myType="chp";
}

Customer::~Customer() = default;

std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    if(!alreadyOrdered()) {
        int min = 100000000; //the price of the cheapest dish
        int minid =-1; //the id of the cheapest dish
        vector<int> v;
        for (const auto &i : menu) {
            if (i.getPrice() < min |(i.getPrice() == min & i.getId()<minid))  {
                min = i.getPrice();
                minid = i.getId();
            }
        }
        if(min!=100000000) {
            setOrder();
            v.push_back(minid);
            cout <<getName()+" ordered "+menu.at((unsigned long)(minid)).getName() << '\n';
            return v;
        }
    }
    return std::vector<int>() ;
}

std::string CheapCustomer::toString() const {
    return std::to_string(this->getId())+" "+this->getName();
}

std::string SpicyCustomer::toString() const {
    return getName()+","+"spc";
}

SpicyCustomer::SpicyCustomer(std::string name, int id): Customer(std::move(name),id){//added
    myType="spc";
}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    int max=0;
    int min=100000000;
    int id=-1;//the id of the chosen dish
    vector<int> v;
    if(!alreadyOrdered()) {
        for (const auto &i : menu) {
            if (i.getType()==SPC & (i.getPrice() > max |(i.getPrice() ==max & i.getId()<id)) ) {
                max = i.getPrice();
                id = i.getId();
            }
        }
        setOrder();
        if(id!=-1){
            cout <<getName()+" ordered "+menu.at((unsigned long)(id)).getName() << '\n';
            v.push_back(id);
            return v;
        }
        return std::vector<int>() ;
    }
    else{
        for (const auto &i : menu) {
            if (i.getType()==BVG &(i.getPrice() < min |(i.getPrice() ==min & i.getId()<id)) ) {
                min = i.getPrice();
                id = i.getId();
            }
        }
        if(id!=-1){
            cout <<getName()+" ordered "+menu.at((unsigned long)(id)).getName() << '\n';
            v.push_back(id);
            return v;
        }
        return std::vector<int>() ;
    }
}

VegetarianCustomer::VegetarianCustomer(std::string name, int id):Customer(std::move(name),id) { //added
    myType="veg";
}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
        int max = 0; //the price of the most expensive BVG
        int minid =100000000; //the id of the veg dish with the smallest id
        int maxid=-1; //the id of the most expensive BVG
        std::vector<int> v;
        for (const auto &i : menu) {
            if (i.getId()<minid & i.getType() == VEG) {
                minid = i.getId();
            }
            if(i.getType()==BVG &(i.getPrice()>max||(i.getPrice()==max& i.getId()<maxid))) {
                maxid= i.getId();
                max= i.getPrice();
            }
        }
        if(minid!=100000000) {
            v.push_back(minid);
            cout <<getName()+" ordered "+menu.at((unsigned long)(minid)).getName() << '\n';
        }
        if(maxid>-1)
            cout<<getName()+" ordered "+menu.at((unsigned long)maxid).getName()<<'\n';
            v.push_back(maxid);
        return v ;
}

std::string VegetarianCustomer::toString() const {
    return std::to_string(this->getId())+" "+this->getName();
}

AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) :Customer(std::move(name),id),alcPrice(-1),alcId(-1){
    myType="alc";
}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    int id=-1;
    int min=10000000;
    vector<int > v;
    for (const auto &i : menu) {
        if (i.getType() == ALC & (i.getPrice()>alcPrice | (i.getPrice()==alcPrice& i.getId()>alcId))
        &(i.getPrice() < min |(i.getPrice() == min & i.getId()<id)))  {
            id= i.getId();
            min= i.getPrice();
        }
    }
    if(id!=-1){
        alcId=id;
        alcPrice=min;
        cout<<getName()+" ordered "+menu.at((unsigned long)id).getName()<<'\n';
        v.push_back(id);
        return v;
    }
    return std::vector<int>();

}

std::string AlchoholicCustomer::toString() const {
    return std::to_string(this->getId())+" "+this->getName();
}
