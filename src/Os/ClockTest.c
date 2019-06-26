#include "Os/ClockMock.c"

#include "Test.h"

Test(OsClockTime)
{
    os_clock_mock_init(12345, 321);
    u64 start = OsClockTime();
    u64 end = OsClockTime();

    _u64(start);
    //=> 12345
    _u64(end - start);
    //=> 321
}
