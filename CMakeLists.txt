cmake_minimum_required(VERSION 2.8.12)

project(client)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(${PROJECT_NAME} "client.cpp")


