#include "test.h"
#include "grid.c"
#include "quad.c"

Test(grid_basics)
{
	int2 h1 = {5, 27};
	int2 h2 = {48, 62};
	Grid *g = malloc(sizeof *g);

	grid_init(g);

	_qd(g->quad);
	//=> (-2047, -2047), (2046, 2046)
	_qd(g->styles_quad);
	//=> (-2048, -2048), (2047, 2047)
	_sq(g->size_quad);
	//=> (-2048, -2048), (4096, 4096)

	grid_set(g, h1, 1);
	grid_set(g, h2, 2);

	_d(grid_get(g, h1));
	//=> 1
	_d(grid_get(g, h2));
	//=> 2

	grid_clear(g, h2);

	_d(grid_get(g, h1));
	//=> 1
	_d(grid_get(g, h2));
	//=> 0

	grid_destroy(g);
	free(g);
}
