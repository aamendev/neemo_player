cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(neemo)

set(GCCCompilerFlags "-std=c++23")
FILE(GLOB SRC
    "main.cc"
    "src/*.cc"
    "src/panes/*.cc"
    "src/tabs/*.cc"
    )

add_executable(${PROJECT_NAME} ${SRC})
set(CMAKE_BUILD_TYPE Debug)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
