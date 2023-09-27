cmake_minimum_required(VERSION 3.25)
project(NetDesign1)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES "config/*.cpp" "server/*.cpp")

include_directories(config server)

add_executable(NetDesign1 main.cpp ${SOURCES})
