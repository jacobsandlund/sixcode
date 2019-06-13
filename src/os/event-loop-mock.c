#include "os/event-loop.h"
#include <stdlib.h>
#include "os/clock.h"

struct OsEventLoop {
    float2 next_incoming_event_location;
    OsEventQueue incoming_events;
};

#define OsEventLoopMockNumEvents 32
#define OsEventLoopMockEventsSafeLengthRemaining 8

void os_event_loop_mock_next_event_location(OsEventLoop *loop, float2 location)
{
    loop->next_incoming_event_location = location;
}

void os_event_loop_mock_event_type(OsEventLoop *loop, OsEventType event_type)
{
    OsEvent incoming_event = {
        .type = event_type,
        .location = loop->next_incoming_event_location,
    };
    os_event_queue_write(&loop->incoming_events, &incoming_event);
}

OsEventLoop *os_event_loop_create(void)
{
    OsEventLoop *loop = malloc(sizeof *loop);
    loop->next_incoming_event_location = (float2) {
        0.0,
        0.0,
    };
    os_event_queue_init(&loop->incoming_events,
            OsEventLoopMockNumEvents,
            OsEventLoopMockEventsSafeLengthRemaining);

    return loop;
}

void os_event_loop_destroy(OsEventLoop *loop)
{
    os_event_queue_destroy(&loop->incoming_events);
    free(loop);
}

void os_event_loop_run(OsEventLoop *loop, OsEventQueue *queue)
{
    OsEventQueue *incoming = &loop->incoming_events;
    OsEvent i_event;

    while (
            os_event_queue_read(incoming, &i_event) > 0 &&
            i_event.type != OsEventTypeTerminateLoop) {
        OsEvent event = {
            .type = i_event.type,
            .time = os_clock_time(),
            .location = i_event.location,
        };
        os_event_queue_write(queue, &event);
    }
}
