#!/bin/bash

COMPILER="g++"
FLAGS="-std=c++14 -pthread -Werror -Wextra -Wpedantic -Wshadow -Wconversion -Wnull-dereference -Wno-unused-parameter"
INCLUDE_DIR="include"

SOURCE="main.cpp"
OUT_DIR="build"
EXECUTABLE="fslog"

mkdir -p $OUT_DIR

$COMPILER $FLAGS $SOURCE -o $OUT_DIR/$EXECUTABLE -I$INCLUDE_DIR

echo "Build complete."