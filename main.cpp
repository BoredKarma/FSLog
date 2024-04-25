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
    fslog::warn("The answer is {}", (void*)&test);

    Custom custom = { 1, 2 };
    fslog::error("The answer is {}", custom);
}
