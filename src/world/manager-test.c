#include "world/manager.c"
#include "math/quad.c"
#include "os/clock-mock.c"
#include "world/grid.c"
#include "test.h"

Test(WorldManagerInit__WorldManagerLoadRandom)
{
    os_clock_mock_init(12345, 321);

    WorldManagerConfig config = {
        .grid_size = 64,
    };

    WorldManagerInit(&config);
    WorldManagerLoadRandom(512);

    _qd(gWorldManager.grid.quad);
    //=> (-32, -32), (31, 31)

    WorldManagerDestroy();
}
