#include "test.h"
#include "world/grid.c"
#include "math/quad.c"

Test(world_grid_basics)
{
	int2 h1 = {5, 27};
	int2 h2 = {48, 62};
	WorldGrid *wg = tmalloc(sizeof *wg);

	world_grid_init(wg);

	_qd(wg->quad);
	//=> (-2047, -2047), (2046, 2046)
	_qd(wg->styles_quad);
	//=> (-2048, -2048), (2047, 2047)
	_sq(wg->size_quad);
	//=> (-2048, -2048), (4096, 4096)

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
