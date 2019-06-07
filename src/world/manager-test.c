#include "test.h"
#include "world/manager.c"
#include "world/grid.c"
#include "math/quad.c"
#include "os/clock-mock.c"

Test(world_manager_init_and_load)
{
    gOsClockMock = (OsClockMock) {.time = 12345};

    world_manager_init();
    world_manager_load_random(128);

    _qd(gWorldManager.grid.quad);
    //=> (-2047, -2047), (2046, 2046)

    world_manager_destroy();
}
