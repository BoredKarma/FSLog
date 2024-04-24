#pragma once

#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <functional>

#include "fslog.h"

namespace fslog {

class threading_logger {
public:
    threading_logger() : terminate_thread_(false) {
        logging_thread_ = std::thread(&threading_logger::thread_func, this);
    }

    ~threading_logger() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            terminate_thread_ = true;
        }
        cond_.notify_one();
        logging_thread_.join();
    }
    
    template<typename... Args>  void debug(const std::string& fmt, Args... args) {
        _log("Debug", fslog::debug_colors, fmt, args...);
    }
    template<typename... Args> void debug(const fslog::CallInfo& call, const std::string& fmt, Args... args) {
        _log("Debug", call, fslog::debug_colors, fmt, args...);
    }

    template<typename... Args> void info(const std::string& fmt, Args... args) {
        _log("INFO", fslog::info_colors, fmt, args...);
    }
    template<typename... Args> void info(const fslog::CallInfo& call, const std::string& fmt, Args... args) {
        _log("INFO", call, fslog::info_colors, fmt, args...);
    }

    template<typename... Args> void warn(const std::string& fmt, Args... args) {
        _log("WARN", fslog::warn_colors, fmt, args...);
    }
    template<typename... Args> void warn(const fslog::CallInfo& call, const std::string& fmt, Args... args) {
        _log("WARN", call, fslog::warn_colors, fmt, args...);
    }

    template<typename... Args> void error(const std::string& fmt, Args... args) {
        _log("ERROR", fslog::error_colors, fmt, args...);
    }
    template<typename... Args> void error(const fslog::CallInfo& call, const std::string& fmt, Args... args) {
        _log("ERROR", call, fslog::error_colors, fmt, args...);
    }

private:
    void thread_func() {
        while (true) {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_.wait(lock, [this] { 
                return !messages_.empty() || terminate_thread_; 
            });

            while (!messages_.empty()) {
                messages_.front()();
                messages_.pop_front();
            }

            if (messages_.empty() && terminate_thread_) {
                return;
            }
        }
    }
    
    template<typename... Args>
    void _log(const std::string& type, const fslog::LogColors& colors, const std::string& fmt, Args... args) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            messages_.push_back([=]() {
                fslog::log(type, colors, fmt, args...);
            });
        }
        cond_.notify_one();
    }
    template<typename... Args>
    void _log(const std::string& type, const fslog::CallInfo& call, const fslog::LogColors& colors, const std::string& fmt, Args... args) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            messages_.push_back([=]() {
                fslog::log(type, call, colors, fmt, args...);
            });
        }
        cond_.notify_one();
    }

private:
    std::deque<std::function<void()>> messages_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::thread logging_thread_;
    bool terminate_thread_;
};

}