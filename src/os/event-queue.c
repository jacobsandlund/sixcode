#include "os/event-queue.h"
#include <assert.h>
#include "log/manager.h"

void os_event_queue_init(OsEventQueue *eq, i64 length, i64 safe_length_remaining)
{
    assert(length && !(length & (length - 1)));    // Power of 2
    eq->next_read_event_id = 1;
    eq->next_write_event_id = 1;
    eq->index_mask = length - 1;
    eq->length = length;
    eq->safe_read_behind = length - safe_length_remaining;
    eq->events = malloc(length * sizeof *eq->events);
}

void os_event_queue_destroy(OsEventQueue *eq)
{
    free(eq->events);
}

i64 os_event_queue_read(OsEventQueue *eq, OsEvent *event)
{
    i64 behind = eq->next_write_event_id - eq->next_read_event_id;

    if (behind == 0) {
        return 0;
    } else if (behind > eq->safe_read_behind) {
        i64 skipping = behind - eq->safe_read_behind;
        LogDefault(&gLogManager.logs.os,
                "Event queue read behind by %" PRId64
                " above safe level of %" PRId64
                ": skipping %" PRId64 " messages",
                behind, eq->safe_read_behind, skipping);
        eq->next_read_event_id += skipping;
    }

    *event = eq->events[eq->next_read_event_id & eq->index_mask];

    return eq->next_read_event_id++;
}
