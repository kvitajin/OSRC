cmake_minimum_required(VERSION 3.18)
project(projektcpp)

find_package(SQLite3)
set(CMAKE_CXXFLAGS "${CMAKE_CXX_FLAGS} -l sqlite3 -lwiringPi -lrt -pthread")


set(CMAKE_CXX_STANDARD 20)

add_executable(projektcpp main.cpp)
target_link_libraries(projektcpp LINK_PUBLIC sqlite3 wiringPi)
