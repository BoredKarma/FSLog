#define FSLOG_DEBUG
#include "fslog.h"
#include "timer.h"

int main(int, char**) {
    // {
    //     Timer timer("10000x fslog::debug()");

    //     for (int i = 0; i < 10000; ++i)
    //         fslog::debug("The answer is {}", 42);
    // }

    int test = 42;
    fslog::debug("The answer is {}", (void*)&test);
}
