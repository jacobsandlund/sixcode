#include "os/clock-mock.c"
#include "test.h"

Test(os_clock_time)
{
    gOsClockMock = (OsClockMock) {.time = 12345, .time_increment = 321};
    u64 start = os_clock_time();
    u64 end = os_clock_time();

    _u64(start);
    //=> 12345
    _u64(end - start);
    //=> 321
}
