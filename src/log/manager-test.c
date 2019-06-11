#include "test.h"
#include "log/manager-mock.c"

Test(log_manager_init)
{
    log_manager_init(&gLogManagerMockConfig);

    LogMock *log_mock = (LogMock *) gLogManager.logs.os.log_impl;
    _s(log_mock->subsystem);
    //=> computer.spacetime-test.os
    _s(log_mock->category);
    //=> default

    log_mock = (LogMock *) gLogManager.logs.engine.log_impl;
    _s(log_mock->subsystem);
    //=> computer.spacetime-test.engine
    _s(log_mock->category);
    //=> default

    log_at_level(&gLogManager.logs.os, LogLevelDefault,
            "This is a test log %s", "foo");

    LogDebug(&gLogManager.logs.os, "Debug this %d", 42);

    _Log(&gLogManager.logs.os);
    //=> [default]  This is a test log foo
    //=> [debug]  Debug this 42
    //=>

    log_manager_destroy();
}
