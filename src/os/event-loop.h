#ifndef OsEventLoop_h
#define OsEventLoop_h

#include "spacetime.h"
#include "os/event-queue.h"

typedef struct {
    void *event_loop_impl;
} OsEventLoop;

void os_event_loop_init(OsEventLoop *event_loop);
void os_event_loop_destroy(OsEventLoop *event_loop);
void os_event_loop_run(OsEventLoop *event_loop, OsEventQueue *queue);

#endif // OsEventLoop_h
