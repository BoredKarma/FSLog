#pragma once

#include <chrono>
#include <iostream>
#include <string>

double time_elapsed = 0;

class ScopeTimer {
public:
    explicit ScopeTimer() : start_(std::chrono::high_resolution_clock::now()) {}
    
    ~ScopeTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start_;
        time_elapsed = elapsed.count();
    }

    ScopeTimer(const ScopeTimer&) = delete;
    ScopeTimer& operator=(const ScopeTimer&) = delete;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};