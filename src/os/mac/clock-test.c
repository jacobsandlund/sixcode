#include "os/mac/clock.c"
#include "test/test.h"

Test(os_clock_time)
{
    u64 start = os_clock_time();
    u64 end = os_clock_time();

    printf("os_clock_time: %" PRIu64 "\n", end);

    _d(start > 0 && end > 0);
    //=> 1
    _d(end > start);
    //=> 1
}
