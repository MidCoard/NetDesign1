cmake_minimum_required(VERSION 3.25)
project(NetDesign1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

find_package(nlohmann_json 3.2.0 REQUIRED)

file(GLOB_RECURSE SOURCES "config/*.cpp" "server/*.cpp" "ui/*.cpp")

if (WIN32)
    add_definitions(-DWIN32)
endif ()

include_directories(config server ui .)

add_executable(NetDesign1 main.cpp ${SOURCES})

target_link_libraries(NetDesign1
        Qt::Core
        Qt::Gui
        Qt::Widgets
        nlohmann_json::nlohmann_json)
