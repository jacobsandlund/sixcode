#include "os/event-loop-mock.c"
#include "os/clock-mock.c"
#include "os/event-queue.c"
#include "log/manager-mock.c"
#include "test.h"

Test(os_event_loop)
{
    os_clock_mock_init(12345, 321);
    log_manager_init(&gLogManagerMockConfig);

    OsEventLoop *loop = os_event_loop_create();

    _f2(loop->next_incoming_event_location);
    //=> 0, 0
    _d(loop->incoming_events.next_read_event_id);
    //=> 1

    float2 location = {1300, 400};
    os_event_loop_mock_next_event_location(loop, location);
    _f2(loop->next_incoming_event_location);
    //=> 1300, 400

    os_event_loop_mock_event_type(loop, OsEventTypeMouseMove);
    location = (float2) {700, 800};
    os_event_loop_mock_next_event_location(loop, location);
    os_event_loop_mock_event_type(loop, OsEventTypeMouseClick);
    os_event_loop_mock_event_type(loop, OsEventTypeTerminateLoop);

    OsEventQueue queue;
    os_event_queue_init(&queue, 32, 4);

    os_event_loop_run(loop, &queue);

    OsEvent event;

    _d(os_event_queue_read(&queue, &event));
    //=> 1
    _d(event.type == OsEventTypeMouseMove);
    //=> 1
    _f2(event.location);
    //=> 1300, 400
    _u64(event.time);
    //=> 12345

    _d(os_event_queue_read(&queue, &event));
    //=> 2
    _d(event.type == OsEventTypeMouseClick);
    //=> 1
    _f2(event.location);
    //=> 700, 800
    _u64(event.time);
    //=> 12666

    os_event_queue_destroy(&queue);
    os_event_loop_destroy(loop);
    log_manager_destroy();
}
