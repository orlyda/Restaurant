//
// Created by adi on 11/6/18.

#include "../include/Action.h"
#include "../include/Restaurant.h"
#include "../include/Table.h"
#include <iostream>
#include "../include/Dish.h"

using namespace std;
extern Restaurant* backup;

BaseAction::~BaseAction() = default;

BaseAction::BaseAction(): status(PENDING),errorMsg(""){}

ActionStatus BaseAction::getStatus() const { return  status;}

std::string BaseAction::printstatus() const {
    if(getStatus()==PENDING){
        return "Pending";
    }
    if(getStatus()==COMPLETED){
        return "Completed";
    }
    if(getStatus()==ERROR){
        return "Error: "+getErrorMsg();
    }
}

std::string BaseAction::getErrorMsg() const { return errorMsg;}

void BaseAction::complete() {status=COMPLETED;}

void BaseAction::error(std::string errorMsg) {
    status=ERROR;
    this->errorMsg=errorMsg;
    cout<< "Error: "+this->errorMsg<<'\n';
}

OpenTable* OpenTable::clone() { return new OpenTable(*this);}

Order* Order::clone() { return new Order(*this);}

MoveCustomer* MoveCustomer::clone() { return new MoveCustomer(*this);}

Close* Close::clone() { return new Close(*this);}

CloseAll* CloseAll::clone() { return new CloseAll(*this);}

PrintActionsLog* PrintActionsLog::clone() { return new PrintActionsLog(*this);}

PrintTableStatus* PrintTableStatus::clone() { return new PrintTableStatus(*this);}

PrintMenu* PrintMenu::clone() { return new PrintMenu(*this);}

BackupRestaurant* BackupRestaurant::clone() { return new BackupRestaurant(*this);}

RestoreResturant* RestoreResturant::clone() { return new RestoreResturant(*this);}


//OpenTable
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):tableId(id), customers(){
    for(auto c:customersList){
       customers.push_back(c->clone());
   }
}

OpenTable::~OpenTable() { //destructor
    for(auto  c:customers){
        if(c!= nullptr){
            delete c;
            c =  nullptr;
        }
    }
    customers.clear();
}


void OpenTable::act(Restaurant &restaurant) {
    if(restaurant.getTable(tableId)== nullptr||restaurant.getTable(tableId)->isOpen()) {
        error("Table does not exist or is already open");
        restaurant.setActionLog(this);
    }
    else{
        for(int i=0;i<customers.size();i++){
            restaurant.getTable(tableId)->addCustomer(customers.at((unsigned long)i));
        }
        restaurant.getTable(tableId)->openTable();
        restaurant.setActionLog(this);
        complete();
    }
}
//
std::string OpenTable::toString() const {
    string output="open "+to_string(tableId)+" ";

    for(auto & c:customers){
       // output+=c->getName()+","+c->getmyType()+" ";
        output+=c->toString();
    }

    return output+" "+printstatus()+"\n";
}
//end OpenTable

//Order
Order::Order(int id):tableId(id){}

void Order::act(Restaurant &restaurant) {
   // string output;
    if(!restaurant.getTable(tableId)->isOpen() || restaurant.getTable(tableId)== nullptr) {
        error("Table does not exist or is already open");
        restaurant.setActionLog(this);
    }
    else{
        restaurant.getTable(tableId)->order(restaurant.getMenu());
        /*
        int customerid;
        string namecustomer;
        string dishcustomer;
        for(int i=0;i<restaurant.getTable(tableId)->getOrders().size();i++){
            customerid=restaurant.getTable(tableId)->getOrders().at(i).first;
            namecustomer=restaurant.getTable(tableId)->getCustomer(customerid)->getName();
            dishcustomer=restaurant.getTable(tableId)->getOrders().at(i).second.getName();
            output+=namecustomer+" ordered "+dishcustomer+"\n";
        }
         */
        restaurant.setActionLog(this);
     //   cout<< output;
        complete();
    }
}

std::string Order::toString() const {
    return "order "+to_string(tableId)+" "+printstatus()+"\n";
}
//end Order

//MoveCustomer
MoveCustomer::MoveCustomer(int src, int dst, int customerId):dstTable(dst),srcTable(src),id(customerId){}

void MoveCustomer::act(Restaurant &restaurant) {
    Table & source =*(restaurant.getTable(srcTable));
    Table & destintion =*(restaurant.getTable(dstTable));
    if(source.getCustomer(id)== nullptr|| !source.isOpen() | !destintion.isOpen() |
            destintion.getCustomers().size()+1>destintion.getCapacity()){
        error("cannot move customer");
        restaurant.setActionLog(this);
    }
    else {
        vector<OrderPair> temp;

        for(auto & s:source.getOrders()){
            temp.push_back(s);
        }
        source.getOrders().clear();

        for(int i=0;i<temp.size();i++){
            if(temp.at((unsigned long)i).first == source.getCustomer(id)->getId()){
                destintion.getOrders().push_back(temp.at((unsigned long)i));
            }
            else {
                source.getOrders().push_back(temp.at((unsigned long)i));
            }
        }
        temp.clear();

        destintion.addCustomer(source.getCustomer(id));
        source.removeCustomer(id);

        if(source.getCustomers().empty()){
            source.closeTable();
        }
        restaurant.setActionLog(this);
        complete();
    }
}

std::string MoveCustomer::toString() const {
    return "move "+to_string(srcTable)+" "+to_string(dstTable)+" "+to_string(id)+" "+printstatus()+"\n";
}
//end MoveCustomer

//Close
Close::Close(int id):tableId(id){}

void Close::act(Restaurant &restaurant) {
    if(restaurant.getTable(tableId)== nullptr || !restaurant.getTable(tableId)->isOpen()) { //??null
        error("Table does not exist or is not open");
        restaurant.setActionLog(this);
    }
    else{
        cout<< "Table "+to_string(tableId)+" was closed. Bill "+to_string(restaurant.getTable(tableId)->getBill())+"NIS\n";
        restaurant.setActionLog(this);
        restaurant.getTable(tableId)->closeTable();
        complete();
    }
}

std::string Close::toString() const {
    return "close "+to_string(tableId)+" "+printstatus()+"\n";
}
//end Close

//Close all
CloseAll::CloseAll(): BaseAction() {}
void CloseAll::act(Restaurant &restaurant) {
    string output;
    for(int i=0;i<restaurant.getNumOfTables();i++){
        if(restaurant.getTable(i)->isOpen()) {
            output += "Table " + to_string(i) + " was closed. Bill " + to_string(restaurant.getTable(i)->getBill()) +
                      "NIS\n";
            restaurant.getTable(i)->closeTable();
        }
    }
    restaurant.closeResturant();
    cout<<output;
    restaurant.setActionLog(this);
    complete();
}

std::string CloseAll::toString() const {
    return "closeall "+printstatus()+"\n";
}
//end Close all

//Print menu
PrintMenu::PrintMenu():BaseAction() {}

void PrintMenu::act(Restaurant &restaurant) {
    string output;
    for (auto &i : restaurant.getMenu()) {
        string dishType;
        if (i.getType()==ALC){
            dishType="ALC";
        }
        else if(i.getType()==SPC){
            dishType="SPC";
        }
        else if(i.getType()==BVG){
            dishType="BVG";
        }
        else if(i.getType()==VEG){
            dishType="VEG";
        }
        output+= i.getName()+" "+dishType+" "+std::to_string(i.getPrice())+"NIS\n";
    }
    cout<< output;
    complete();
}

std::string PrintMenu::toString() const {
    return "menu "+printstatus()+"\n";
}
//end Print menu

//Print Table status
PrintTableStatus::PrintTableStatus(int id):tableId(id) {
}

void PrintTableStatus::act(Restaurant &restaurant) {
    ///?>>
    if(restaurant.getTable(tableId)->isOpen()){
        Table thetable =*(restaurant.getTable(tableId));
        cout<<"Table "+to_string(tableId)+" status: open\n";
        cout<<"Customers:\n";
        for(int i=0;i<thetable.getCustomers().size();i++){
         cout<< to_string(thetable.getCustomers().at((unsigned long)i)->getId()) + " " +
                thetable.getCustomers().at((unsigned long)(i))->getName() + "\n";
        }
        cout<<"Orders:\n";
        for (auto &i : thetable.getOrders()) {
           cout<< i.second.getName()+" "+
           std::to_string(i.second.getPrice())+"NIS "+
           std::to_string(i.first)+"\n";
        }
        cout<<"Current Bill:"+to_string(thetable.getBill())+"NIS"+"\n";
    }
    else{
        cout<<"Table "+to_string(tableId)+" status: closed\n";
    }
    restaurant.setActionLog(this);
    complete();
}

std::string PrintTableStatus::toString() const {
    return "status "+to_string(tableId)+" "+printstatus()+"\n";
}
//end Table status

//Print Action log
PrintActionsLog::PrintActionsLog():BaseAction() {}
void PrintActionsLog::act(Restaurant &restaurant) {
    string status;

    for (auto i : restaurant.getActionsLog()) {
        cout << i->toString();
    }
    restaurant.setActionLog(this);
    complete();
}

std::string PrintActionsLog::toString() const {
    return "log "+printstatus()+"\n";
}
//end Action log

//Backup Restaurant
BackupRestaurant::BackupRestaurant():BaseAction(){}

void BackupRestaurant::act(Restaurant &restaurant) {
    if (backup != nullptr) {
        //backup=&restaurant;
        delete(backup);
        backup= nullptr;
    }
    //else
    backup = new Restaurant(restaurant); //// ?????
  // *backup=restaurant;

    restaurant.setActionLog(this);
    complete();
}

std::string BackupRestaurant::toString() const {
    return "backup "+printstatus()+"\n";
}
// end Backup Restaurant

//Restore Restaurant
RestoreResturant::RestoreResturant():BaseAction(){}

void RestoreResturant::act(Restaurant &restaurant) {
    if(backup== nullptr) {
        BaseAction::error("No backup available");
        restaurant.setActionLog(this);
    }
    else {
        restaurant = *backup;
        restaurant.setActionLog(this);
        complete();
    }
}

std::string RestoreResturant::toString() const {
    return "restore "+printstatus()+"\n";
}
//end Restore Restaurant
///