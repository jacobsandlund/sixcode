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
    OsEventQueueWrite(&loop->incoming_events, &incoming_event);
}

OsEventLoop *OsEventLoopCreate(void)
{
    OsEventLoop *loop = malloc(sizeof *loop);
    loop->next_incoming_event_location = (float2) {
        0.0,
        0.0,
    };
    OsEventQueueInit(&loop->incoming_events,
            OsEventLoopMockNumEvents,
            OsEventLoopMockEventsSafeLengthRemaining);

    return loop;
}

void OsEventLoopDestroy(OsEventLoop *loop)
{
    OsEventQueueDestroy(&loop->incoming_events);
    free(loop);
}

void OsEventLoopRun(OsEventLoop *loop, OsEventQueue *queue)
{
    OsEventQueue *incoming = &loop->incoming_events;
    OsEvent i_event;

    while (
            OsEventQueueRead(incoming, &i_event) > 0 &&
            i_event.type != OsEventTypeTerminateLoop) {
        OsEvent event = {
            .type = i_event.type,
            .time = OsClockTime(),
            .location = i_event.location,
        };
        OsEventQueueWrite(queue, &event);
    }
}
