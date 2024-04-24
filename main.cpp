#include "fslog.h"
#include "timer.h"

int main(int, char**) {
    {
        Timer timer("test");

        for (int i = 0; i < 1000; ++i)
            printf("The answer is %d", 42);
    }
}
