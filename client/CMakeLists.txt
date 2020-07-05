cmake_minimum_required(VERSION 3.15)
project(client)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads)
add_executable(client main.cpp src/client/Client.cpp src/client/Client.h src/state/State.cpp src/state/State.h src/state/StateMain.cpp src/state/StateMain.h src/state/StateNewMessage.cpp src/state/StateNewMessage.h src/state/StateUploadFile.cpp src/state/StateUploadFile.h src/state/StateDownloadFile.cpp src/state/StateDownloadFile.h)
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})