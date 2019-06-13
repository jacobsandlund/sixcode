#include "os/clock.h"

typedef struct {
    u64 time;
    u64 time_increment;
} OsClockMock;

OsClockMock gOsClockMock;

void os_clock_mock_init(u64 time, u64 time_increment)
{
    gOsClockMock.time = time;
    gOsClockMock.time_increment = time_increment;
}

u64 os_clock_time(void)
{
    u64 time = gOsClockMock.time;
    gOsClockMock.time += gOsClockMock.time_increment;
    return time;
}
