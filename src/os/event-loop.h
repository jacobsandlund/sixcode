#ifndef OsEventLoop_h
#define OsEventLoop_h

#include "spacetime.h"
#include "os/event-queue.h"

typedef struct OsEventLoop OsEventLoop;

OsEventLoop *os_event_loop_create(void);
void os_event_loop_destroy(OsEventLoop *loop);
void os_event_loop_run(OsEventLoop *loop, OsEventQueue *queue);

#endif // OsEventLoop_h
