cmake_minimum_required(VERSION 3.12)
project(Restaurant)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(src)

add_executable(Restaurant
    include/Action.h
    include/Customer.h
    include/Dish.h
    include/Restaurant.h
    include/Table.h
    src/Action.cpp
    src/Customer.cpp
    src/Dish.cpp
    src/Main.cpp
    src/Restaurant.cpp
    src/Table.cpp
    config1.txt)
