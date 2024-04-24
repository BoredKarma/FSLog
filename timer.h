#pragma once

#include <chrono>
#include <iostream>
#include <string>

std::chrono::duration<double, std::milli> elapsed;

class Timer {
public:
    explicit Timer(const std::string& name) : name_(name), start_(std::chrono::high_resolution_clock::now()) {}

    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        elapsed = end - start_;
        std::cout << name_ << ": " << elapsed.count() << "ms" << std::endl;
        //total += elapsed.count();
    }

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

private:
    std::string name_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};