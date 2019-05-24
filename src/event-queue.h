#ifndef EventQueue_h
#define EventQueue_h

#include "spacetime.h"

typedef enum {
	EventTypeMouseMove = 1,
	EventTypeMouseDrag = 2,
	EventTypeMouseClick = 4,
} EventType;

#define EventTypeMouseMoveOrDrag 3

typedef struct {
	EventType type;
	u64 time;
	float2 location;
} Event;

typedef struct {
	i64 next_read_event_id;
	i64 next_write_event_id;
	i64 index_mask;
	i64 length;
	i64 safe_read_behind;
	Event *events;
} EventQueue;

void event_queue_init(EventQueue *eq, i64 length, i64 safe_length_remaining);
void event_queue_destroy(EventQueue *eq);
u64 event_queue_clock_time(void);

static inline void event_queue_write(EventQueue *eq, Event *event)
{
	eq->events[eq->next_write_event_id++ & eq->index_mask] = *event;
}

i64 event_queue_read(EventQueue *eq, Event *event);

#endif // EventQueue_h
