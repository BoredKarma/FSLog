cmake_minimum_required(VERSION 3.10)
project(fslog VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Werror -Wextra -Wpedantic -Wshadow -Wconversion -Wnull-dereference -Wno-unused-parameter)
    # if(UNIX)
    #     set(THREADS_PREFER_PTHREAD_FLAG ON)
    #     find_package(Threads REQUIRED)
    #     link_libraries(Threads::Threads)
    # endif()
endif()

include_directories(include)
set(SOURCE_FILES main.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

message("Build complete.")