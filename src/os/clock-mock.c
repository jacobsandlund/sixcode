#include "os/clock.h"

typedef struct {
	u64 time;
	u64 time_increment;
} OsClockMock;

OsClockMock gOsClockMock;

u64 os_clock_time(void)
{
	u64 time = gOsClockMock.time;
	gOsClockMock.time += gOsClockMock.time_increment;
	return time;
}
