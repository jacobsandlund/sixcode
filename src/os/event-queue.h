#ifndef OsEventQueue_h
#define OsEventQueue_h

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

void os_event_queue_init(OsEventQueue *eq, i64 length, i64 safe_length_remaining);
void os_event_queue_destroy(OsEventQueue *eq);
i64 os_event_queue_read(OsEventQueue *eq, OsEvent *event);

static inline void os_event_queue_write(OsEventQueue *eq, OsEvent *event)
{
	eq->events[eq->next_write_event_id++ & eq->index_mask] = *event;
}

#endif // OsEventQueue_h
