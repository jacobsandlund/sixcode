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

    log_mock = (LogMock *) gLogManager.logs.world.log_impl;
    _s(log_mock->subsystem);
    //=> computer.spacetime-test.world
    _s(log_mock->category);
    //=> default

    log_manager_destroy();
}
