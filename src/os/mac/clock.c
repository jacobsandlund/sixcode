#include "os/clock.h"
#include <time.h>

u64 OsClockTime(void)
{
    return clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
}
