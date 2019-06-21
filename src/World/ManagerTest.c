#include "World/Manager.c"
#include "Math/Quad.c"
#include "Os/ClockMock.c"
#include "World/Grid.c"
#include "Test.h"

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
