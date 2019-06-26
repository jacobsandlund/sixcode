#include "Log/Mac/Log.m"

#include "Test.h"

Test(log)
{
    LogConfig config = {
        .subsystem = "computer.spacetime-test.testing",
        .category = "default",
    };
    Log *log = LogCreate(&config);

    LogAtLevel(log, LogLevelDebug, "Just for debugging, you should know...");
    LogAtLevel(log, LogLevelInfo, "Got some info");
    LogAtLevel(log, LogLevelDefault, "This looks bad");
    LogAtLevel(log, LogLevelError, "A Foo error has occurred");
}
