#include "world/manager.c"
#include "math/quad.c"
#include "os/clock-mock.c"
#include "world/grid.c"
#include "test/test.h"

Test(world_manager_init_and_load)
{
    os_clock_mock_init(12345, 321);

    WorldManagerConfig config = {
        .grid_size = 64,
    };

    world_manager_init(&config);
    world_manager_load_random(512);

    _qd(gWorldManager.grid.quad);
    //=> (-32, -32), (31, 31)

    world_manager_destroy();
}
