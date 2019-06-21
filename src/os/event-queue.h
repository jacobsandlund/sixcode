#ifndef _OsEventQueue_h
#define _OsEventQueue_h

#include "spacetime.h"
#include "os/event.h"

typedef struct {
    i64 next_read_event_id;
    i64 next_write_event_id;
    i64 index_mask;
    i64 length;
    i64 safe_read_behind;
    OsEvent *events;
} OsEventQueue;

void OsEventQueueInit(OsEventQueue *eq, i64 length, i64 safe_length_remaining);
void OsEventQueueDestroy(OsEventQueue *eq);
i64 OsEventQueueRead(OsEventQueue *eq, OsEvent *event);

static inline void OsEventQueueWrite(OsEventQueue *eq, OsEvent *event)
{
    eq->events[eq->next_write_event_id++ & eq->index_mask] = *event;
}

#endif // _OsEventQueue_h
