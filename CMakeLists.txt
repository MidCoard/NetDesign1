cmake_minimum_required(VERSION 3.25)
project(NetDesign1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

set(CMAKE_AUTOMOC ON)

set(PYTHON_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/plot.py")

# 拷贝Python脚本到构建目录
configure_file(${PYTHON_SCRIPT} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

# 引入JSON 解析库
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

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
