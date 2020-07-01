cmake_minimum_required(VERSION 3.15)
project(server)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads)
add_executable(server main.cpp src/server/Server.cpp src/server/Server.h src/mediator/Mediator.cpp src/mediator/Mediator.h src/client/Client.cpp src/client/Client.h)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})