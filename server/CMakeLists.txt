cmake_minimum_required(VERSION 3.15)
project(server)

set(CMAKE_CXX_STANDARD 17)

add_executable(server main.cpp src/server/Server.cpp src/server/Server.h)