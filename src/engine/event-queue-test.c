#include "test.h"
#include "engine/event-queue.c"

Test(event_queue_init)
{
	EventQueue *eq = malloc(sizeof *eq);

	event_queue_init(eq, 64, 4);

	_d(eq->next_read_event_id);
	//=> 1
	_d(eq->next_write_event_id);
	//=> 1
	_d(eq->index_mask);
	//=> 63
	_d(eq->length);
	//=> 64
	_d(eq->safe_read_behind);
	//=> 60

	eq->events[eq->length - 1].type = EventTypeMouseClick;

	event_queue_destroy(eq);
	free(eq);
}

Test(event_queue_write)
{
	EventQueue *eq = malloc(sizeof *eq);

	Event event = {
		.type = EventTypeMouseClick,
		.time = 1234567890,
		.location = {1000, 800},
	};

	event_queue_init(eq, 4, 1);

	event_queue_write(eq, &event);

	_d(eq->next_write_event_id);
	//=> 2
	_d(eq->events[1].type == event.type);
	//=> 1
	_u64(eq->events[1].time);
	//=> 1234567890
	_f2(eq->events[1].location);
	//=> 1000, 800

	// Write past length
	event_queue_write(eq, &event);
	event_queue_write(eq, &event);
	event_queue_write(eq, &event);
	event.time = 9876543210;
	event_queue_write(eq, &event);

	_d(eq->next_write_event_id);
	//=> 6
	_u64(eq->events[1].time);
	//=> 9876543210

	event_queue_destroy(eq);
	free(eq);
}

Test(event_queue_read)
{
	EventQueue *eq = malloc(sizeof *eq);

	Event event = {
		.type = EventTypeMouseClick,
		.time = 1234567890,
		.location = {1000, 800},
	};
	Event read_event;

	event_queue_init(eq, 4, 2);
	event_queue_write(eq, &event);
	event_queue_write(eq, &event);

	_d(event_queue_read(eq, &read_event));
	//=> 1

	_d(eq->next_read_event_id);
	//=> 2
	_d(read_event.type == event.type);
	//=> 1
	_u64(read_event.time);
	//=> 1234567890
	_f2(read_event.location);
	//=> 1000, 800

	_d(event_queue_read(eq, &read_event));
	//=> 2

	// Read far behind

	event_queue_write(eq, &event);	// skip read
	event.time = 9876543210;
	event_queue_write(eq, &event);
	event.time = 1234567890;
	event_queue_write(eq, &event);

	_d(event_queue_read(eq, &read_event));
	//=> 4

	_Log();
	//=> Event queue read behind by 3 above safe level of 2: skipping 1 messages
	_u64(read_event.time);
	//=> 9876543210

	event_queue_destroy(eq);
	free(eq);
}

Test(event_queue_clock_time)
{
	u64 start = event_queue_clock_time();
	u64 end = event_queue_clock_time();

	_d(start > 0 && end > 0);
	//=> 1
	_d(end > start);
	//=> 1
}
