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
	Event *events;
} EventQueue;

void event_queue_initialize(EventQueue *eq, i64 length);
void event_queue_terminate(EventQueue *eq);
u64 event_queue_clock_time(void);

static inline void event_queue_write(EventQueue *eq, Event *event)
{
	eq->events[eq->next_write_event_id++ & eq->index_mask] = *event;
}

static inline void event_queue_read(EventQueue *eq, Event *event)
{
	*event = eq->events[eq->next_read_event_id++ & eq->index_mask];
}

#endif // EventQueue_h
