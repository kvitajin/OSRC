cmake_minimum_required(VERSION 3.18)
project(projektcpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(projektcpp main.cpp)

find_package(SQLite3 REQUIRED)
find_library(WIRINGPI_LIBRARIES NAMES wiringPi REQUIRED)
find_package(Threads REQUIRED)
find_library(LIBRT rt REQUIRED)

target_link_libraries(projektcpp PUBLIC SQLite::SQLite3 ${WIRINGPI_LIBRARIES} ${LIBRT} Threads::Threads)