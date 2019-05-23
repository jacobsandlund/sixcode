#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "spacetime.h"

typedef enum {
	EVENT_TYPE_MOUSE_MOVE = 1,
	EVENT_TYPE_MOUSE_DRAG = 2,
	EVENT_TYPE_MOUSE_CLICK = 4,
} EventType;

#define EVENT_TYPE_MOUSE_MOVE_OR_DRAG_MASK 3

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

static inline void event_queue_write(EventQueue *eq, Event *event)
{
	eq->events[eq->next_write_event_id++ & eq->index_mask] = *event;
}

static inline void event_queue_read(EventQueue *eq, Event *event)
{
	*event = eq->events[eq->next_read_event_id++ & eq->index_mask];
}

#endif // EVENT_QUEUE_H
