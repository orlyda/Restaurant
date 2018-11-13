//
// Created by adi on 11/6/18.
//
#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include "Dish.h"
#include "Table.h"
#include "Action.h"
#include <iostream>
#include <fstream>
#include <string>


class Restaurant{
public:
    Restaurant();
    Restaurant(Restaurant &restaurant); ///copy constructor
    Restaurant(Restaurant &&other); //move constructor
   // Restaurant&Restaurant::operator={Restaurant && other};
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    void setActionLog(BaseAction* action);//added
    std::vector<Dish>& getMenu();
    std::vector<std::string> split(const std::string &s,char d); ///added
    void closeResturant();///added
    int getCostumerId ();
    void setCostumerId();
private:
    int costId;
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif