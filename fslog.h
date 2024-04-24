#pragma once

#include <string>
#include <chrono>
#include <vector>
#include <array>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#elif defined(__linux__)
    #include <unistd.h>
    #include <sys/syscall.h>
#else
    #error "Unsupported platform :("
#endif

#if defined(_MSC_VER)
    #define INLINE __forceinline
    #define FS_FUNC __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
    #define INLINE __attribute__((always_inline)) inline
    #define FS_FUNC __PRETTY_FUNCTION__
#else
    #define INLINE inline
    #error "HAHAJSHSHAAHA!!!!"
#endif

#define FS_LINE __LINE__
#define FS_META fslog::CallInfo{__fs_get_file_name(__FILE__), FS_LINE}
#define FS_POINT FS_DEBUG, "{}", FS_FUNC

#define FSLOG_PROCESS inline std::string process
#define FSLOG_PROCESS_DEFAULT(type) FSLOG_PROCESS(type arg) { return std::to_string(arg); }

INLINE std::string __fs_get_file_name(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    return (pos != std::string::npos) ? path.substr(pos + 1) : path;
}

enum class FsColor {
    #if defined(_WIN32) || defined(_WIN64)
        RED = FOREGROUND_RED,
        BLUE = FOREGROUND_BLUE,
        GREEN = FOREGROUND_GREEN,
        WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        GRAY = FOREGROUND_INTENSITY,
        YELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
        CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
    #elif defined(__linux__)
            RED = 31,
            BLUE = 34,
            GREEN = 32,
            WHITE = 37,
            GRAY = 90,
            YELLOW = 33,
            CYAN = 36,
    #endif
};

namespace fslog {
    struct LogColors {
        FsColor bracket = FsColor::WHITE;
        FsColor prefix = FsColor::WHITE;
        FsColor text = FsColor::WHITE;
    };
    struct CallInfo {
        std::string file;
        int line;
    };

    // You can edit these as you like.
    static LogColors debug_colors = { FsColor::GRAY, FsColor::BLUE, FsColor::WHITE };
    static LogColors info_colors = { FsColor::GRAY, FsColor::CYAN, FsColor::WHITE };
    static LogColors warn_colors = { FsColor::GRAY, FsColor::YELLOW, FsColor::WHITE };
    static LogColors error_colors = { FsColor::GRAY, FsColor::RED, FsColor::WHITE };

    namespace types {
        FSLOG_PROCESS_DEFAULT(int32_t)
        FSLOG_PROCESS_DEFAULT(uint32_t)
        FSLOG_PROCESS_DEFAULT(int64_t)
        FSLOG_PROCESS_DEFAULT(uint64_t)
        FSLOG_PROCESS_DEFAULT(double)
        FSLOG_PROCESS_DEFAULT(float)

        FSLOG_PROCESS(bool arg) { return std::string(arg ? "true" : "false"); }
        FSLOG_PROCESS(const std::string& arg) { return arg; }
        FSLOG_PROCESS(const char* arg) { return arg; }

        FSLOG_PROCESS(void* arg) {
            char buffer[32] = { 0 };
            std::snprintf(buffer, sizeof(buffer), "%p",
                reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(arg))
            );
            return std::string(buffer);
        }

        // FSLOG_PROCESS(const CustomType& arg) { return std::string(arg.integer_member); }
        // FSLOG_PROCESS(Unity::System_String* arg) { return arg->ToString(); }
    } // types

    namespace {
        INLINE void fs_write(const char* str, const size_t& length) {
            #if defined(_WIN32) || defined(_WIN64)
                WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), str, static_cast<DWORD>(length), nullptr, nullptr);
            #elif defined(__linux__)
                syscall(SYS_write, STDOUT_FILENO, str, length);
            #endif
        }

        INLINE void setcolor(FsColor color) {
            #if defined(_WIN32) || defined(_WIN64)
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<int>(color));
            #elif defined(__linux__)
                printf("\033[%dm", static_cast<int>(color));
            #endif
        }

        INLINE void fs_localtime(const time_t *t, struct tm* timeinfo) {
            #if defined(_WIN32) || defined(_WIN64)
                localtime_s(timeinfo, t);
            #elif defined(__linux__)
                localtime_r(t, timeinfo);
            #endif
        }

        std::string get_time() {
            char buf[32] = { 0 };
            time_t now_time;
            time(&now_time);

            struct tm timeinfo;
            fs_localtime(&now_time, &timeinfo);
            strftime(buf, sizeof(buf), "%H:%M:%S", &timeinfo);

            return buf;
        }

        void p_brackets(const std::string& fmt, FsColor bracket, FsColor text) {
            setcolor(bracket);  fs_write("[", 1);
            setcolor(text);     fs_write(fmt.c_str(), fmt.length());
            setcolor(bracket);  fs_write("] ", 2);
        }
    } // details

    namespace fmt {
        template<typename... Args>
        std::string format(const std::string& fmt, Args... args) {
            const size_t num_args = sizeof...(args);
            if (!num_args) return fmt;

            std::string result;
            result.reserve(fmt.length() + num_args * 10);
            const std::array<std::string, sizeof...(args)> fmt_args = { 
                types::process(args)... 
            };

            size_t arg_index = 0;
            for (size_t i = 0; i < fmt.length(); ++i) {
                if (fmt[i] == '{' && i + 1 < fmt.length() && fmt[i + 1] == '}') {
                    if (arg_index < num_args) {
                        result += fmt_args[arg_index++];
                        i++;
                    }
                    else {
                        result += "{";
                    }
                }
                else {
                    result += fmt[i];
                }
            }

            return result;
        }
    } // fmt

    template<typename... Args>
    void log(const std::string& type, const LogColors& colors, const std::string& fmt, Args... args) {
        std::string formatted = fmt::format(fmt, args...) + '\n';
        p_brackets(get_time(), colors.bracket, colors.prefix);
        p_brackets(type, colors.bracket, colors.prefix);

        setcolor(colors.text);
        fs_write(formatted.c_str(), formatted.length());
        setcolor(FsColor::WHITE);
    }
    template<typename... Args>
    void log(const std::string& type, const CallInfo& call, const LogColors& colors, const std::string& fmt, Args... args) {
        std::string formatted = fmt::format(fmt, args...) + '\n';
        p_brackets(get_time(), colors.bracket, colors.prefix);
        p_brackets(type, colors.bracket, colors.prefix);
        p_brackets(fmt::format("{}:{}", call.file, call.line), colors.bracket, colors.prefix);

        setcolor(colors.text);
        fs_write(formatted.c_str(), formatted.length());
        setcolor(FsColor::WHITE);
    }

    template<typename... Args> void debug(const std::string& fmt, Args... args) {
        log("Debug", debug_colors, fmt, args...);
    }
    template<typename... Args> void debug(const CallInfo& call, const std::string& fmt, Args... args) {
        log("Debug", call, debug_colors, fmt, args...);
    }

    template<typename... Args> void info(const std::string& fmt, Args... args) {
        log("INFO", info_colors, fmt, args...);
    }
    template<typename... Args> void info(const CallInfo& call, const std::string& fmt, Args... args) {
        log("INFO", call, info_colors, fmt, args...);
    }

    template<typename... Args> void warn(const std::string& fmt, Args... args) {
        log("WARN", warn_colors, fmt, args...);
    }
    template<typename... Args> void warn(const CallInfo& call, const std::string& fmt, Args... args) {
        log("WARN", call, warn_colors, fmt, args...);
    }

    template<typename... Args> void error(const std::string& fmt, Args... args) {
        log("ERROR", error_colors, fmt, args...);
    }
    template<typename... Args> void error(const CallInfo& call, const std::string& fmt, Args... args) {
        log("ERROR", call, error_colors, fmt, args...);
    }
} // fslog