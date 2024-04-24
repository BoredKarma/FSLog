#define FSLOG_DEBUG
#include "fslog.h"

std::string between_brackets(const std::string& str, const fslog::LogColors& colors) {
    return fslog::fmt::format("{}[{}{}{}]{}", 
           fslog::setcolor(colors.bracket), fslog::setcolor(colors.prefix), str, fslog::setcolor(colors.bracket), fslog::setcolor(FsColor::WHITE)
    );
}

int main(int, char**) {
    // fslog::debug("Your answer is {}", 42);

    // fslog::LogColors colors = fslog::debug_colors;
    // std::string message = "Heewo";
    
    // std::string prefixes = fslog::fmt::format("{} {} {}{}",
    //     between_brackets("DEBUG", colors), between_brackets("DEBUG", colors), fslog::setcolor(colors.text), message
    // );
    // printf("%s", prefixes.c_str());

    fslog::debug("The answer is {}", 42);
    fslog::debug(FS_META, "The answer is {}", 42);
}
