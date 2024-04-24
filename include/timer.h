#pragma once

#include <chrono>
#include <iostream>
#include <string>

#include "fslog.h"

class Timer {
public:
    explicit Timer(const std::string& name) : name_(name), start_(std::chrono::high_resolution_clock::now()) {}

    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start_;
        fslog::info("{}: {}ms", name_, elapsed.count());
    }

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

private:
    std::string name_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};