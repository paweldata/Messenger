cmake_minimum_required(VERSION 3.15)
project(server)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads)
add_executable(server main.cpp src/server/Server.cpp src/server/Server.h src/mediator/Mediator.cpp src/mediator/Mediator.h src/client/Client.cpp src/client/Client.h)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})

# gtest
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

add_executable(test test.cpp src/server/Server.cpp src/server/Server.h src/mediator/Mediator.cpp src/mediator/Mediator.h src/client/Client.cpp src/client/Client.h)
target_link_libraries(test gtest_main)
add_test(NAME example_test COMMAND example)
