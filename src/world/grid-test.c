#include "world/grid.c"
#include "math/quad.c"
#include "test.h"

Test(world_grid_basics)
{
    int2 h1 = {5, 27};
    int2 h2 = {48, 62};
    WorldGrid *wg = tmalloc(sizeof *wg);

    WorldGridInit(wg, 128);

    _qd(wg->quad);
    //=> (-64, -64), (63, 63)
    _sq(wg->size_quad);
    //=> (-64, -64), (128, 128)

    WorldGridSet(wg, h1, 1);
    WorldGridSet(wg, h2, 2);

    _d(WorldGridGet(wg, h1));
    //=> 1
    _d(WorldGridGet(wg, h2));
    //=> 2

    WorldGridClear(wg, h2);

    _d(WorldGridGet(wg, h1));
    //=> 1
    _d(WorldGridGet(wg, h2));
    //=> 0

    WorldGridDestroy(wg);
}
