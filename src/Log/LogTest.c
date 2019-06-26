#include "Log/LogMock.c"

#include "Test.h"

Test(log)
{
    LogConfig config = {
        .subsystem = "computer.spacetime-test.test",
        .category = "default",
    };
    Log *log = LogCreate(&config);

    _s(log->subsystem);
    //=> computer.spacetime-test.test
    _s(log->category);
    //=> default

    LogAtLevel(log, LogLevelDebug, "Just for debugging, you should know...");
    LogAtLevel(log, LogLevelInfo, "Got some info");
    LogAtLevel(log, LogLevelDefault, "This looks bad");
    LogAtLevel(log, LogLevelError, "A Foo error has occurred");

    _Log(log);
    //=> [debug]  Just for debugging, you should know...
    //=> [info]  Got some info
    //=> [default]  This looks bad
    //=> [error]  A Foo error has occurred
    //=>

    LogDestroy(log);
}
