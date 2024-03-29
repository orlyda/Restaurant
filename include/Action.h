//
// Created by adi on 11/6/18.
//

#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"
#include "Table.h"

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
    virtual BaseAction* clone()=0;
    virtual ~BaseAction();
    std::string printstatus() const;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    void act(Restaurant &restaurant);
    std::string toString() const;
    OpenTable* clone();
    ~OpenTable();//destructor
private:
    const int tableId;
    std::vector<Customer *> customers;
};

class Order : public BaseAction {
public:
    Order(int id);
    Order* clone();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    MoveCustomer* clone();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Restaurant &restaurant);
    Close* clone();
    std::string toString() const;
private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    CloseAll* clone();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    PrintMenu* clone();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    PrintTableStatus* clone();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    PrintActionsLog* clone();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    BackupRestaurant* clone();
    void act(Restaurant &restaurant);
    std::string toString() const;
private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
    RestoreResturant* clone();
    void act(Restaurant &restaurant);
    std::string toString() const;
};

#endif