#include "log/manager-mock.c"
#include "test.h"

Test(log_manager_init)
{
    log_manager_init(&gLogManagerMockConfig);

    _s(gLogManager.logs.os->subsystem);
    //=> computer.spacetime-test.os
    _s(gLogManager.logs.os->category);
    //=> default

    _s(gLogManager.logs.engine->subsystem);
    //=> computer.spacetime-test.engine
    _s(gLogManager.logs.engine->category);
    //=> default

    log_at_level(gLogManager.logs.os, LogLevelDefault,
            "This is a test log %s", "foo");

    LogDebug(gLogManager.logs.os, "Debug this %d", 42);

    _Log(gLogManager.logs.os);
    //=> [default]  This is a test log foo
    //=> [debug]  Debug this 42
    //=>

    log_manager_destroy();
}
