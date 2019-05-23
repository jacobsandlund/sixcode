#include <assert.h>
#include "event-queue.h"

void event_queue_initialize(EventQueue *eq, i64 length)
{
	assert(length && !(length & (length - 1)));	// Power of 2
	eq->next_read_event_id = 0;
	eq->next_write_event_id = 0;
	eq->index_mask = length - 1;
	eq->length = length;
	eq->events = malloc(length * sizeof *eq->events);
}

void event_queue_terminate(EventQueue *eq)
{
	free(eq->events);
}
