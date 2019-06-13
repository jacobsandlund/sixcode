#include "log/mac/log.m"
#include "test.h"

Test(log)
{
    LogConfig config = {
        .subsystem = "computer.spacetime-test.testing",
        .category = "default",
    };
    Log *log = log_create(&config);

    log_at_level(log, LogLevelDebug, "Just for debugging, you should know...");
    log_at_level(log, LogLevelInfo, "Got some info");
    log_at_level(log, LogLevelDefault, "This looks bad");
    log_at_level(log, LogLevelError, "A Foo error has occurred");
}
