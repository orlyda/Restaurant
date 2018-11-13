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
Restaurant::Restaurant():open(false){}

//copy constructor
Restaurant::Restaurant(Restaurant &restaurant):open(restaurant.open),costId(restaurant.costId)
,tables(restaurant.tables),menu(restaurant.menu) {
    for(auto i:restaurant.actionsLog){
        actionsLog.push_back(i->clone());
    }
    for (auto j:restaurant.tables){
        tables.push_back(j->clone());
    }
}

std::vector<Table*>& Restaurant::getAllTables() { return tables;} ////added

void Restaurant::closeResturant() {open= false;}

void Restaurant::setActionLog(BaseAction* action) { ///added
    actionsLog.push_back(action);
}

const std::vector<BaseAction*>& Restaurant::getActionsLog() const { return actionsLog;}

Restaurant::Restaurant(const std::string &configFilePath) {
    ifstream myfile(configFilePath);
    string line;
    int numberOfTabels(0);
    std::vector<string>tablesdescription;
    std::vector<string>menudescription;
    int idmenu(0);
    string name;
    int price;

    if (myfile.is_open()){
        while (getline(myfile,line)){
            if(line=="#number of tables"){
                getline(myfile,line);
                numberOfTabels=std::stoi(line);
                tables.resize(numberOfTabels);
            }
            if(line=="#tables description"){
                getline(myfile,line);
                tablesdescription=split(line,',');
                for(int i=0;i<=tables.size();i++){
                    Table *table=new Table(std::stoi(tablesdescription.at(i)));
                    tables.push_back(table);
                }
                getline(myfile, line);
            }
            if(line=="#Menu") {
                menudescription = split(line,',');
                name = menudescription.at(0);
                price = std::stoi(menudescription.at(2));

                if (menudescription.at(0) == "ALC") {
                    Dish dish(idmenu, name, price, ALC);
                    menu.push_back(dish);
                }
                else if (menudescription.at(0) == "BVG") {
                    Dish dish(idmenu, name, price, BVG);
                    menu.push_back(dish);
                }
                else if (menudescription.at(0) == "VEG") {
                    Dish dish(idmenu, name, price, VEG);
                    menu.push_back(dish);
                }
                else if (menudescription.at(0) == "SPC") {
                    Dish dish(idmenu, name, price, SPC);
                    menu.push_back(dish);
                }
                idmenu++;
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
    cout << "Restaurant is now open!";
    string input;
    cin >> input;

    while(input!="closeall") { /// not sure at all, need to read more about getting input in a loop
        vector<string> theallinput;
        ifstream theinput(input);
        string str;

        while (theinput >> str) { // Will read up to eof() and stop at every whitespace it hits.
            theallinput.push_back(str);
        }

        if (theallinput.at(0) == "open") { //opentable *** not finished
            int id = stoi(theallinput.at(1));
            vector<Customer *> customerlist;

            for (int i = 2; i < theallinput.size(); i++) {
                vector<string> customerandtype = split(theallinput.at(i), ',');
                if (customerandtype.at(1) == "veg") {
                    VegetarianCustomer vegetarianCustomerustomer(customerandtype.at(0), getCostumerId());
                    setCostumerId();
                    customerlist.push_back(&vegetarianCustomerustomer);
                } else if (customerandtype.at(1) == "spc") {
                    SpicyCustomer spicyCustomer(customerandtype.at(0), getCostumerId());
                    setCostumerId();
                    customerlist.push_back(&spicyCustomer);
                } else if (customerandtype.at(1) == "alc") {
                    AlchoholicCustomer alchoholicCustomer(customerandtype.at(0), getCostumerId());
                    setCostumerId();
                    customerlist.push_back(&alchoholicCustomer);
                } else if (customerandtype.at(1) == "chp") {
                    CheapCustomer cheapCustomer(customerandtype.at(0), getCostumerId());
                    setCostumerId();
                    customerlist.push_back(&cheapCustomer);
                }
            }

            OpenTable openTable(id,customerlist);
            openTable.act(*this);
        }
        else if (theallinput.at(0) == "order") { //order
            int id = stoi(theallinput.at(1));
            Order order(id);
            order.act(*this); //why only works with * ??
        }
        else if (theallinput.at(0) == "move") {//move customer
            int src=stoi(theallinput.at(1));
            int dts=stoi(theallinput.at(2));
            int id=stoi(theallinput.at(3));
            MoveCustomer moveCustomer(src,dts,id);
            moveCustomer.act(*this);
        }
        else if (theallinput.at(0) == "close") {//close
            int id=stoi(theallinput.at(1));
            Close close(id);
            close.act(*this);
        }
        else if (theallinput.at(0) == "menu") { //print menu
            PrintMenu printMenu;
            printMenu.act(*this);
        }
        else if (theallinput.at(0) == "status") { //print table status
            int id=stoi(theallinput.at(0));
            PrintTableStatus printTableStatus(id);
            printTableStatus.act(*this);
        }
        else if (theallinput.at(0) == "log") { //print actions log
            PrintActionsLog printActionsLog;
            printActionsLog.act(*this);
        }
        else if (theallinput.at(0) == "backup") { //backup resturant
            BackupRestaurant backupRestaurant;
            backupRestaurant.act(*this);
        }
        else if (theallinput.at(0) == "restore") {//restore resturant
            RestoreResturant restoreResturant;
            restoreResturant.act(*this);
        }
        cin>>input;
    }
    if (input == "closeall") {//closeall
        CloseAll closeAll;
        closeAll.act(*this);
    }
}

int Restaurant::getNumOfTables() const { return tables.size();}

Table* Restaurant::getTable(int ind) {
    if (ind<tables.size() | ind>tables.size())
    {
        return nullptr;
    }
    else {
        return tables.at(ind);
    }
}

std::vector<Dish>& Restaurant::getMenu() { return menu;}