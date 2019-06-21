#include "Os/EventQueue.c"
#include "Log/ManagerMock.c"
#include "Test.h"

Test(OsEventQueueInit)
{
    OsEventQueue *eq = tmalloc(sizeof *eq);

    OsEventQueueInit(eq, 64, 4);

    _d(eq->next_read_event_id);
    //=> 1
    _d(eq->next_write_event_id);
    //=> 1
    _d(eq->index_mask);
    //=> 63
    _d(eq->length);
    //=> 64
    _d(eq->safe_read_behind);
    //=> 60

    eq->events[eq->length - 1].type = OsEventTypeMouseClick;

    OsEventQueueDestroy(eq);
}

Test(OsEventQueueWrite)
{
    LogManagerInit(&gLogManagerMockConfig);

    OsEventQueue *eq = tmalloc(sizeof *eq);

    OsEvent event = {
        .type = OsEventTypeMouseClick,
        .time = 1234567890,
        .location = {1000, 800},
    };

    OsEventQueueInit(eq, 4, 1);

    OsEventQueueWrite(eq, &event);

    _d(eq->next_write_event_id);
    //=> 2
    _d(eq->events[1].type == event.type);
    //=> 1
    _u64(eq->events[1].time);
    //=> 1234567890
    _f2(eq->events[1].location);
    //=> 1000, 800

    // Write past length
    OsEventQueueWrite(eq, &event);
    OsEventQueueWrite(eq, &event);
    OsEventQueueWrite(eq, &event);
    event.time = 9876543210;
    OsEventQueueWrite(eq, &event);

    _d(eq->next_write_event_id);
    //=> 6
    _u64(eq->events[1].time);
    //=> 9876543210

    OsEventQueueDestroy(eq);
    log_manager_destroy();
}

Test(OsEventQueueRead)
{
    LogManagerInit(&gLogManagerMockConfig);

    OsEventQueue *eq = tmalloc(sizeof *eq);

    OsEvent event = {
        .type = OsEventTypeMouseClick,
        .time = 1234567890,
        .location = {1000, 800},
    };
    OsEvent read_event;

    OsEventQueueInit(eq, 4, 2);
    OsEventQueueWrite(eq, &event);
    OsEventQueueWrite(eq, &event);

    _d(OsEventQueueRead(eq, &read_event));
    //=> 1

    _d(eq->next_read_event_id);
    //=> 2
    _d(read_event.type == event.type);
    //=> 1
    _u64(read_event.time);
    //=> 1234567890
    _f2(read_event.location);
    //=> 1000, 800

    _d(OsEventQueueRead(eq, &read_event));
    //=> 2

    // Read far behind

    OsEventQueueWrite(eq, &event);    // skip read
    event.time = 9876543210;
    OsEventQueueWrite(eq, &event);
    event.time = 1234567890;
    OsEventQueueWrite(eq, &event);

    _d(OsEventQueueRead(eq, &read_event));
    //=> 4

    _Log(gLogManager.logs.os);
    //=> [default]  Event queue read behind by 3 above safe level of 2: skipping 1 messages
    //=>
    _u64(read_event.time);
    //=> 9876543210

    OsEventQueueDestroy(eq);
    log_manager_destroy();
}
