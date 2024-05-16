#pragma once

#include <chrono>
#include <iostream>
#include <string>

class ScopeTimer {
public:
    explicit ScopeTimer(double& time_elapsed) : time_elapsed_(time_elapsed), start_(std::chrono::high_resolution_clock::now()) {}
    
    ~ScopeTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start_;
        time_elapsed_ += elapsed.count();
    }

    ScopeTimer(const ScopeTimer&) = delete;
    ScopeTimer& operator=(const ScopeTimer&) = delete;

private:
    double& time_elapsed_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};