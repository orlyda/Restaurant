//
// Created by adi on 11/6/18.
//
#include "../include/Restaurant.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include "../include/Action.h"

using namespace std;
//constructor
Restaurant::Restaurant():costId(0),open(false), tables(),menu(),actionsLog(),numOfTables(0){}

//copy constructor
Restaurant::Restaurant(Restaurant &restaurant):costId(restaurant.costId),open(restaurant.open),
tables(),menu(restaurant.menu), actionsLog() {
    for(auto i:restaurant.actionsLog){
        actionsLog.push_back(i->clone());
    }
    for (auto j:restaurant.tables){
        tables.push_back(j->clone());
    }
}
//move construstor
/*
Restaurant::Restaurant(Restaurant &&other):costId(other.costId),open(other.open),
tables(other.tables),menu(),actionsLog(other.actionsLog){
    for (const auto &i : other.menu) {
        menu.push_back(i);
    }
    other.menu.clear();
    other.tables.clear();
    other.actionsLog.clear();
    other.costId=0;
    other.open=false;
}
 */

/*
//move assignment operator
Restaurant& Restaurant::operator=(Restaurant  &&other) {
    if(this!=&other){
        menu.clear();
        tables.clear();
        actionsLog.clear();
        menu.reserve(other.menu.size());
        tables.reserve(other.tables.size());
        actionsLog.reserve(other.tables.size());
        costId=other.costId;
        open=other.open;
        actionsLog=other.actionsLog;
        tables=other.tables;
        for (const auto &i : other.menu) {
            menu.push_back(i);
        }

    }
    return *this;
}
 */

//copy assignment operator
Restaurant& Restaurant::operator=(const Restaurant &other) {
    if (this != &other) {
        open = other.open;
        numOfTables = other.numOfTables;
        costId = other.open;

        for(auto &a:actionsLog){
            delete a;
        }
        actionsLog.clear();

        for (auto &i:other.actionsLog) {
            actionsLog.push_back(i->clone());
        }
        // for(unsigned int j=0;j<tables.size();j++){
        //     tables[j]=other.tables[j];
        //  }
        for (auto &t:tables) {
            delete t;
        }
        tables.clear();

        for (auto &ot:other.tables) {
            tables.push_back(ot->clone());
        }
        menu.clear();
        for(auto & om:other.menu){
            menu.push_back(om);
        }
    }
    return *this;
}

//destructor
Restaurant::~Restaurant() {
    for(auto& i:actionsLog) {
        if(i!= nullptr) {
            delete (i);
            i = nullptr;
        }
    }
    actionsLog.clear();

    for(auto& j:tables) {
        if(j!= nullptr) {
            delete j;
            j = nullptr;
        }
    }
    tables.clear();
    menu.clear();
}

void Restaurant::closeResturant() {open= false;}

void Restaurant::setActionLog(BaseAction* action) { ///added
    actionsLog.push_back(action);
}

const std::vector<BaseAction*>& Restaurant::getActionsLog() const { return actionsLog;}

Restaurant::Restaurant(const std::string &configFilePath):
costId(0),open(false), tables(),menu(),actionsLog() {
    ifstream myfile(configFilePath);
    string line;
    int numberOfTabels(0);
    std::vector<string>tablesdescription;
    std::vector<string>menudescription;
    int idmenu=0;
    string name;
    int price;
 //   int whichLine=0;
    if (myfile.is_open()){
        while (getline(myfile,line)){
            if(line.empty()||line=="\r") continue;
            if(line=="#number of tables"||line=="#number of tables\r"){
                getline(myfile,line);
                if(line.substr(line.size()-1)=="\r") {
                    numberOfTabels = std::stoi(line.substr(0, line.size() - 1));
                    numOfTables = numberOfTabels;
                }
                else{
                    numberOfTabels=std::stoi(line);
                    numOfTables = numberOfTabels;
                }
                getline(myfile,line);
            }
            if(line=="#tables description"||line=="#tables description\r"){
                getline(myfile,line);
                if(line.empty()||line=="\r") continue;
                if(line.substr(line.size()-1)=="\r")
                    line=line.substr(0,line.size()-1);
                tablesdescription = split(line, ',');
                    for (auto i = 0; i < numberOfTabels; i++) {
                        tables.push_back(new Table(std::stoi(tablesdescription.at((unsigned long)i))));
                    }
                getline(myfile,line);

            }
            if(line=="#Menu"||line=="#Menu\r") {
                while (getline(myfile,line)) {
                    if(line.empty()||line=="\r") continue;
                    if(line.substr(line.size()-1)=="\r")
                        line=line.substr(0,line.size()-1);
                    menudescription = split(line, ',');
                    name = menudescription.at(0);
                    price = std::stoi(menudescription.at(2));

                    if (menudescription.at(1) == "ALC") {
                        Dish dish(idmenu, name, price, ALC);
                        menu.push_back(dish);
                    } else if (menudescription.at(1) == "BVG") {
                        Dish dish(idmenu, name, price, BVG);
                        menu.push_back(dish);
                    } else if (menudescription.at(1) == "VEG") {
                        Dish dish(idmenu, name, price, VEG);
                        menu.push_back(dish);
                    } else if (menudescription.at(1) == "SPC") {
                        Dish dish(idmenu, name, price, SPC);
                        menu.push_back(dish);
                    }
                    idmenu++;
                }
            }
        }
    }
    myfile.close();
}


std::vector<std::string> Restaurant::split(const std::string &s, char d) {
    std::vector<std::string> parts;
    std::string part;
    std::istringstream partsStream(s);
    while (std::getline(partsStream,part,d)) {
        parts.push_back(part);
    }
    return parts;
}

void Restaurant::setCostumerId() { costId++; }

int Restaurant::getCostumerId() { return costId;}

void Restaurant::start() {
    open = true;
    cout << "Restaurant is now open!\n";
    string input;
    getline(cin,input);

    while(input!="closeall") {
        vector<string> theallinput=split(input,' ');

        if (theallinput.at(0) == "open") { //opentable
            int id = stoi(theallinput.at(1));
            vector<Customer *> customerlist;

            for (unsigned int i = 2; i < theallinput.size(); i++) {
                vector<string> customerandtype = split(theallinput.at((unsigned long)i), ',');
                if (customerandtype.at(1) == "veg") {
                    VegetarianCustomer *vegetarianCustomerustomer=new VegetarianCustomer(customerandtype.at(0), getCostumerId());
                    setCostumerId();
                    customerlist.push_back(vegetarianCustomerustomer);
                } else if (customerandtype.at(1) == "spc") {
                    SpicyCustomer *spicyCustomer=new SpicyCustomer(customerandtype.at(0), getCostumerId());
                    setCostumerId();
                    customerlist.push_back(spicyCustomer);
                } else if (customerandtype.at(1) == "alc") {
                    AlchoholicCustomer *alchoholicCustomer=new  AlchoholicCustomer(customerandtype.at(0), getCostumerId());
                    setCostumerId();
                    customerlist.push_back(alchoholicCustomer);
                } else if (customerandtype.at(1) == "chp") {
                    CheapCustomer *cheapCustomer=new CheapCustomer(customerandtype.at(0), getCostumerId());
                    setCostumerId();
                    customerlist.push_back(cheapCustomer);
                }
            }
            auto openTable=new OpenTable(id,customerlist);
            openTable->act(*this);
            for(auto c:customerlist) {
                delete c;
            }
            customerlist.clear();
        }
        else if (theallinput.at(0) == "order") { //order
            int id = stoi(theallinput.at(1));
            auto order=new Order(id);
            order->act(*this);
        }
        else if (theallinput.at(0) == "move") {//move customer
            int src=stoi(theallinput.at(1));
            int dts=stoi(theallinput.at(2));
            int id=stoi(theallinput.at(3));
            auto moveCustomer=new MoveCustomer(src,dts,id);
            moveCustomer->act(*this);
        }
        else if (theallinput.at(0) == "close") {//close
            int id=stoi(theallinput.at(1));
            auto *close=new Close(id);
            close->act(*this);
        }
        else if (theallinput.at(0) == "menu") { //print menu
            auto *printMenu = new PrintMenu();
            printMenu->act(*this);
        }
        else if (theallinput.at(0) == "status") { //print table status
            int id=stoi(theallinput.at(1));
            auto *printTableStatus=new PrintTableStatus(id);
            printTableStatus->act(*this);
        }
        else if (theallinput.at(0) == "log") { //print actions log
            auto *printActionsLog=new PrintActionsLog();
            printActionsLog->act(*this);
        }
        else if (theallinput.at(0) == "backup") { //backup resturant

            auto *backupRestaurant=new  BackupRestaurant();
            backupRestaurant->act(*this);
        }
        else if (theallinput.at(0) == "restore") {//restore resturant
            auto *restoreResturant=new  RestoreResturant();
            restoreResturant->act(*this);
        }
        getline(cin,input);
    }
    if (input == "closeall") {//closeall
        auto *closeAll=new CloseAll();
        closeAll->act(*this);
//        delete(this);
    }
}

int Restaurant::getNumOfTables() const { return numOfTables;}

Table* Restaurant::getTable(int ind) {
    if ((unsigned)ind>tables.size() || ind<0)
    {
        return nullptr;
    }
    else {
        return tables.at((unsigned long)(ind));
    }
}

std::vector<Dish>& Restaurant::getMenu() { return menu;}