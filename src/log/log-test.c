#include "log/log-mock.c"
#include "test.h"

Test(log)
{
    LogConfig config = {
        .subsystem = "computer.spacetime-test.test",
        .category = "default",
    };
    Log *log = log_create(&config);

    _s(log->subsystem);
    //=> computer.spacetime-test.test
    _s(log->category);
    //=> default

    log_at_level(log, LogLevelDebug, "Just for debugging, you should know...");
    log_at_level(log, LogLevelInfo, "Got some info");
    log_at_level(log, LogLevelDefault, "This looks bad");
    log_at_level(log, LogLevelError, "A Foo error has occurred");

    _Log(log);
    //=> [debug]  Just for debugging, you should know...
    //=> [info]  Got some info
    //=> [default]  This looks bad
    //=> [error]  A Foo error has occurred
    //=>
}
