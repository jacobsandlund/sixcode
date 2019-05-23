#include <time.h>
#include "event-queue.c"
#include "test.h"

TEST(event_queue_initialize)
{
	EventQueue *eq = malloc(sizeof *eq);

	event_queue_initialize(eq, 64);

	_d(eq->next_read_event_id);
	//=> 0
	_d(eq->next_write_event_id);
	//=> 0
	_d(eq->index_mask);
	//=> 63
	_d(eq->length);
	//=> 64

	eq->events[eq->length - 1].type = EVENT_TYPE_MOUSE_CLICK;

	event_queue_terminate(eq);
	free(eq);
}

TEST(event_queue_write)
{
	EventQueue *eq = malloc(sizeof *eq);

	Event event = {
		.type = EVENT_TYPE_MOUSE_CLICK,
		.time = 1234567890,
		.location = {1000, 800},
	};

	event_queue_initialize(eq, 16);

	event_queue_write(eq, &event);

	_d(eq->next_write_event_id);
	//=> 1
	_d(eq->events[0].type == event.type);
	//=> 1
	_u64(eq->events[0].time);
	//=> 1234567890
	_f2(eq->events[0].location);
	//=> 1000, 800

	event_queue_terminate(eq);
	free(eq);
}

TEST(event_queue_read)
{
	EventQueue *eq = malloc(sizeof *eq);

	Event event = {
		.type = EVENT_TYPE_MOUSE_CLICK,
		.time = 1234567890,
		.location = {1000, 800},
	};
	Event read_event;

	event_queue_initialize(eq, 16);

	event_queue_write(eq, &event);
	event_queue_read(eq, &read_event);

	_d(eq->next_read_event_id);
	//=> 1
	_d(read_event.type == event.type);
	//=> 1
	_u64(read_event.time);
	//=> 1234567890
	_f2(read_event.location);
	//=> 1000, 800

	event_queue_terminate(eq);
	free(eq);
}
