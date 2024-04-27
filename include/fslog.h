#pragma once

#include <string>
#include <chrono>
#include <vector>
#include <array>
#include <cinttypes>
#include <typeinfo>

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
    #error "Unsupported compiler :("
#endif

#define FS_LINE __LINE__
#define FS_META fslog::CallInfo{__fs_get_file_name(__FILE__), FS_LINE}
#define FS_POINT FS_DEBUG, "{}", FS_FUNC

#define FSLOG_PROCESS inline std::string process
#define FSLOG_PROCESS_DEFAULT(type) FSLOG_PROCESS(type arg) { return std::to_string(arg); }

#ifdef FSLOG_DEBUG
    #define FSLOG_DEBUG_PRINT(x) printf("fslog.h: " x "\n")
#else
    #define FSLOG_DEBUG_PRINT(x)
#endif

INLINE std::string __fs_get_file_name(const std::string& path) {
    size_t pos = path.find_last_of("/\\");
    return (pos != std::string::npos) ? path.substr(pos + 1) : path;
}

enum class FsColor {
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37,
    GRAY = 90,
    BRIGHT_BLACK = 90,
    BRIGHT_RED = 91,
    BRIGHT_GREEN = 92,
    BRIGHT_YELLOW = 93,
    BRIGHT_BLUE = 94,
    BRIGHT_MAGENTA = 95,
    BRIGHT_CYAN = 96,
    BRIGHT_WHITE = 97
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

    static bool has_setup = false;
    
    void setup() {
        #if defined(_WIN32) || defined(_WIN64)
            if (!has_setup) {
                FSLOG_DEBUG_PRINT("Setting up");

                HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
                if (hOut == INVALID_HANDLE_VALUE) {
                    printf("fslog.h: Unable to get console handle\n");
                    return;
                }

                DWORD dwMode = 0;
                if (!GetConsoleMode(hOut, &dwMode)) {
                    printf("fslog.h: Unable to get console mode\n");
                    return;
                }

                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                if (!SetConsoleMode(hOut, dwMode)) {
                    printf("fslog.h: Unable to set console mode\n");
                    return;
                }
                
                has_setup = true;
            }
            else {
                FSLOG_DEBUG_PRINT("Already setup");
            }
        #elif defined(__linux__)
            has_setup = true;
        #endif
    }

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
            if (arg == nullptr) {
                return "0x0";
            }

            uintptr_t value = reinterpret_cast<uintptr_t>(arg);
            const char hex_digits[] = "0123456789abcdef";

            char result[20] = {'\0'};
            int pos = sizeof(result) / sizeof(char) - 2;

            do {
                result[pos--] = hex_digits[value % 16];
                value /= 16;
            } while (value != 0);

            result[pos--] = 'x';
            result[pos--] = '0';

            return std::string(result + pos + 1);
        }
        // FSLOG_PROCESS(const CustomType& arg) { return std::string(arg.integer_member); }
        // FSLOG_PROCESS(Unity::System_String* arg) { return arg->ToString(); }

        template <typename T>
        FSLOG_PROCESS(const T& arg) {
            static_assert(!std::is_pointer<T>::value, "FSLOG_PROCESS: Unknown pointer type");
            static_assert(!std::is_array<T>::value, "FSLOG_PROCESS: Unknown array type");
            static_assert(!std::is_enum<T>::value, "FSLOG_PROCESS: Unknown enum type");
            static_assert(!std::is_class<T>::value, "FSLOG_PROCESS: Unknown class type");
            static_assert(!std::is_union<T>::value, "FSLOG_PROCESS: Unknown union type");
            static_assert(!std::is_reference<T>::value, "FSLOG_PROCESS: Unknown reference type");
            static_assert(!std::is_void<T>::value, "FSLOG_PROCESS: Unknown void type");
            static_assert(!std::is_function<T>::value, "FSLOG_PROCESS: Unknown function type");
            static_assert(!std::is_floating_point<T>::value, "FSLOG_PROCESS: Unknown floating point type");
            static_assert(!std::is_integral<T>::value, "FSLOG_PROCESS: Unknown integral type");
            return "unknown_type";
        }
    } // types

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
    
    namespace {
        INLINE void fs_write(const char* str, const size_t& length) {
            #if defined(_WIN32) || defined(_WIN64)
                WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), str, static_cast<DWORD>(length), nullptr, nullptr);
            #elif defined(__linux__)
                syscall(SYS_write, STDOUT_FILENO, str, length);
            #endif
        }

        INLINE std::string setcolor(FsColor color) {
            return "\033[" + std::to_string(static_cast<int>(color)) + "m";
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

        INLINE std::string p_brackets(const std::string& fmt, const LogColors& colors) {
            return fmt::format("{}[{}{}{}]{}", 
                fslog::setcolor(colors.bracket), fslog::setcolor(colors.prefix), fmt, fslog::setcolor(colors.bracket), fslog::setcolor(FsColor::WHITE)
            );
        }
    } // details

    template<typename... Args>
    void log(const std::string& type, const LogColors& colors, const std::string& fmt, Args... args) {
        if (!has_setup) { 
            fslog::setup(); 
        }

        std::string formatted = fslog::fmt::format("{} {} {}{}\n",
            p_brackets(get_time(), colors), p_brackets(type, colors), fslog::setcolor(colors.text), fslog::fmt::format(fmt, args...)
        );
        fs_write(formatted.c_str(), formatted.length());
    }
    template<typename... Args>
    void log(const std::string& type, const CallInfo& call, const LogColors& colors, const std::string& fmt, Args... args) {
        if (!has_setup) { 
            fslog::setup(); 
        }

        std::string formatted = fslog::fmt::format("{} {} {} {}{}\n",
            p_brackets(get_time(), colors), p_brackets(type, colors), p_brackets(fmt::format("{}:{}", call.file, call.line), colors), 
            fslog::setcolor(colors.text), fslog::fmt::format(fmt, args...)
        );
        fs_write(formatted.c_str(), formatted.length());
    }

    template<typename... Args> void debug(const std::string& fmt, Args... args) {
        log("DEBUG", debug_colors, fmt, args...);
    }
    template<typename... Args> void debug(const CallInfo& call, const std::string& fmt, Args... args) {
        log("DEBUG", call, debug_colors, fmt, args...);
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