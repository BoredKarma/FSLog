cmake_minimum_required(VERSION 3.10)
project(fslog VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Werror -Wextra -Wpedantic -Wshadow -Wconversion -Wnull-dereference -Wno-shorten-64-to-32 -Wno-variadic-macros")

add_executable(fslog main.cpp)

