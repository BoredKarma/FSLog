#include "fslog.h"
#include "fslog_thread.h"

int main(int, char**) {
    {
        fslog::threading_logger logger;

        for (int i = 0; i < 1000; i++) {
            logger.debug(FS_META, "Hello, this is {}!", i);
        }
    } // logger.~threading_logger()

    fslog::debug("Done, this should be printed last.");
}
