#define FSLOG_DEBUG
#include "fslog.h"
#include "timer.h"

struct Custom {
    int a, b;
};

int main(int, char**) {
    {
        Timer timer("1000x fslog::debug()");

        for (int i = 0; i < 1000; ++i)
            fslog::debug("The answer is {}", i);
    }

    int test = 42;
    void* test2 = (void*)0x100;
    void* test3 = (void*)9223372036854775807;
    fslog::warn("test:  {}", (void*)&test);
    fslog::warn("test2: {}", test2);
    fslog::warn("test3: {}", test3);

    fslog::error(FS_META, "Problem!");
    fslog::error(FS_POINT);

    // Custom custom = { 1, 2 };
    // fslog::error("The answer is {}", custom); error: static assertion failed: FSLOG_PROCESS: Unknown class type
}