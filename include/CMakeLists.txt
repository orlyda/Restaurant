cmake_minimum_required(VERSION 3.12)
project(include C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(include
        Action.h
        Customer.h
        Dish.h
        Restaurant.h
        Table.h)
