#include "Os/Mac/Clock.c"

#include "Test.h"

Test(OsClockTime)
{
    u64 start = OsClockTime();
    u64 end = OsClockTime();

    printf("OsClockTime: %" PRIu64 "\n", end);

    _d(start > 0 && end > 0);
    //=> 1
    _d(end > start);
    //=> 1
}
