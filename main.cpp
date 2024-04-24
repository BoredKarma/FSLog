#define FSLOG_DEBUG
#include "fslog.h"
#include "timer.h"

int main(int, char**) {
    {
        Timer timer("test");

        for (int i = 0; i < 1000; ++i)
            fslog::debug("The answer is {}", 42);
    }
}
