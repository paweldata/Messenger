cmake_minimum_required(VERSION 3.15)
project(client)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads)
add_executable(client main.cpp src/client/Client.cpp src/client/Client.h src/client/ClientState.cpp)
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})