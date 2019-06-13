#include "os/screen-mock.c"
#include "test/test.h"

Test(os_screen)
{
    float2 size = {2560, 1440};
    os_screen_mock_init(size);

    OsScreenFrame frame = os_screen_visible_frame();

    _f2(frame.origin);
    //=> 0, 0
    _f2(frame.size);
    //=> 2560, 1440
}
