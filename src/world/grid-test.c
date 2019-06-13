#include "world/grid.c"
#include "math/quad.c"
#include "test/test.h"

Test(world_grid_basics)
{
    int2 h1 = {5, 27};
    int2 h2 = {48, 62};
    WorldGrid *wg = tmalloc(sizeof *wg);

    world_grid_init(wg, 128);

    _qd(wg->quad);
    //=> (-64, -64), (63, 63)
    _sq(wg->size_quad);
    //=> (-64, -64), (128, 128)

    world_grid_set(wg, h1, 1);
    world_grid_set(wg, h2, 2);

    _d(world_grid_get(wg, h1));
    //=> 1
    _d(world_grid_get(wg, h2));
    //=> 2

    world_grid_clear(wg, h2);

    _d(world_grid_get(wg, h1));
    //=> 1
    _d(world_grid_get(wg, h2));
    //=> 0

    world_grid_destroy(wg);
}
