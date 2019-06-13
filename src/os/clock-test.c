#include "os/clock-mock.c"
#include "test/test.h"

Test(os_clock_time)
{
    os_clock_mock_init(12345, 321);
    u64 start = os_clock_time();
    u64 end = os_clock_time();

    _u64(start);
    //=> 12345
    _u64(end - start);
    //=> 321
}
