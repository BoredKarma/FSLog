#pragma once

#include <string>
#include <array>
#include <inttypes.h>

namespace fslog {
    namespace exp_fmt {
        namespace {
            bool all_digits(const std::string& str) {
                if (str.empty()) return false;
                for (char c : str) {
                    if (!::isdigit(c))
                        return false;
                }
                return true;
            }
        }

        template<typename... Args>
        std::string _format(const std::string& fmt, Args&&... args) {
            const size_t num_args = sizeof...(args);
            if (num_args == 0) return fmt;

            const std::array<std::string, num_args> fmt_args = { 
                fslog::types::process(std::forward<Args>(args))... 
            };

            size_t fmt_size = fmt.length();
            std::string result;
            result.reserve(fmt_size + num_args * 10);

            for (size_t i = 0; i < fmt_size; ++i) {
                if (fmt[i] != '{') {
                    result += fmt[i];
                    continue;
                }

                size_t j = i + 1;
                std::string placeholder;
                const int SEARCH_MAXIMUM_LENGTH = 10;
                
                while (j < fmt.length() && fmt[j] != '}' && placeholder.length() < SEARCH_MAXIMUM_LENGTH) {
                    placeholder += fmt[j++];
                }
                if (j == fmt.length() || fmt[j] != '}' || placeholder.length() == SEARCH_MAXIMUM_LENGTH) {
                    result += fmt[i];
                    continue;
                }

                if (all_digits(placeholder)) {
                    size_t arg_index = (size_t)std::stoi(placeholder);
                    result += (arg_index < num_args) ? fmt_args[arg_index] : "{" + placeholder + "}";
                } else {
                    if (placeholder.empty()) {
                        result += "{}";
                    } else {
                        result += "TBI";
                    }
                }

                i = j;
            }

            return result;
        }
    }
} // fmt