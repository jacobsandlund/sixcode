#include "test.h"
#include "os/event-queue.c"

Test(os_event_queue_init)
{
	OsEventQueue *eq = tmalloc(sizeof *eq);

	os_event_queue_init(eq, 64, 4);

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

	eq->events[eq->length - 1].type = OsEventTypeMouseClick;

	os_event_queue_destroy(eq);
}

Test(os_event_queue_write)
{
	OsEventQueue *eq = tmalloc(sizeof *eq);

	OsEvent event = {
		.type = OsEventTypeMouseClick,
		.time = 1234567890,
		.location = {1000, 800},
	};

	os_event_queue_init(eq, 4, 1);

	os_event_queue_write(eq, &event);

	_d(eq->next_write_event_id);
	//=> 2
	_d(eq->events[1].type == event.type);
	//=> 1
	_u64(eq->events[1].time);
	//=> 1234567890
	_f2(eq->events[1].location);
	//=> 1000, 800

	// Write past length
	os_event_queue_write(eq, &event);
	os_event_queue_write(eq, &event);
	os_event_queue_write(eq, &event);
	event.time = 9876543210;
	os_event_queue_write(eq, &event);

	_d(eq->next_write_event_id);
	//=> 6
	_u64(eq->events[1].time);
	//=> 9876543210

	os_event_queue_destroy(eq);
}

Test(os_event_queue_read)
{
	OsEventQueue *eq = tmalloc(sizeof *eq);

	OsEvent event = {
		.type = OsEventTypeMouseClick,
		.time = 1234567890,
		.location = {1000, 800},
	};
	OsEvent read_event;

	os_event_queue_init(eq, 4, 2);
	os_event_queue_write(eq, &event);
	os_event_queue_write(eq, &event);

	_d(os_event_queue_read(eq, &read_event));
	//=> 1

	_d(eq->next_read_event_id);
	//=> 2
	_d(read_event.type == event.type);
	//=> 1
	_u64(read_event.time);
	//=> 1234567890
	_f2(read_event.location);
	//=> 1000, 800

	_d(os_event_queue_read(eq, &read_event));
	//=> 2

	// Read far behind

	os_event_queue_write(eq, &event);	// skip read
	event.time = 9876543210;
	os_event_queue_write(eq, &event);
	event.time = 1234567890;
	os_event_queue_write(eq, &event);

	_d(os_event_queue_read(eq, &read_event));
	//=> 4

	_Log();
	//=> Event queue read behind by 3 above safe level of 2: skipping 1 messages
	//=>
	_u64(read_event.time);
	//=> 9876543210

	os_event_queue_destroy(eq);
}
