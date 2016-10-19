cmake_minimum_required(VERSION 3.6)
project(NetzwerkProgrammierungWS1617)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    bootstrap
    configure.ac
    echo_client.c
    echo_server.c
    Makefile.am
    README.md)

add_executable(NetzwerkProgrammierungWS1617 ${SOURCE_FILES})