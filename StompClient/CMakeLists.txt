cmake_minimum_required(VERSION 3.15)
project(StompClient)

set(CMAKE_CXX_STANDARD 11)

add_executable(StompClient src/main.cpp src/connectionHandler.cpp include/connectionHandler.h)