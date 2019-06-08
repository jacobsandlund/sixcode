#include "test.h"
#include "log/mac/log.m"

Test(log)
{
    Log log;
    LogConfig config = {
        .subsystem = "computer.spacetime-test.testing",
        .category = "default",
    };
    log_init(&log, &config);

    log_at_level(&log, LogLevelDebug, "Just for debugging, you should know...");
    log_at_level(&log, LogLevelInfo, "Got some info");
    log_at_level(&log, LogLevelDefault, "This looks bad");
    log_at_level(&log, LogLevelError, "A Foo error has occurred");
}
