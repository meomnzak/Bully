cmake_minimum_required(VERSION 3.17)
project(Task)

set(CMAKE_CXX_STANDARD 14)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(Task main.cpp Facilitator.cpp Facilitator.h Process.cpp Process.h Message.cpp Message.h)