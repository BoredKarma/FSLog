@echo off

set COMPILER=g++
set FLAGS=-std=c++14 -Werror -Wextra -Wpedantic -Wshadow -Wconversion -Wnull-dereference -Wno-unused-parameter
set SOURCE=main.cpp
set INCLUDE_DIR=include

set OUT_DIR=build
set EXECUTABLE=fslog

if not exist %OUT_DIR% mkdir %OUT_DIR%

%COMPILER% %FLAGS% %SOURCE% -o %OUT_DIR%/%EXECUTABLE% -I %INCLUDE_DIR%

echo Build complete.