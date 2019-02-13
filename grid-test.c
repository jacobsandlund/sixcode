#include <assert.h>
#include "test.h"
#include "grid.c"
#include "hex.c"
#include "quad.c"
#include "space.c"

TEST(grid_quad_to_styles_quad)
{
	Quad q = {{0, 23}, {125, 63}};
	grid_quad_to_styles_quad(&q, &q);
	_qd(q);
	//=> (-128, 0), (127, 127)
}

TEST(grid_basics)
{
	Hex h1 = {5, 27};
	Hex h2 = {48, 62};
	Quad quad = {{0, 0}, {127, 63}};
	Grid *g = malloc(sizeof *g);

	grid_initialize(g, &quad);

	_qd(g->quad);
	//=> (-126, -63), (253, 126)
	_qd(g->styles_quad);
	//=> (-128, -64), (255, 127)
	_sq(g->storage_quad);
	//=> (-64, -64), (192, 192)

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

	grid_terminate(g);
	free(g);
}

TEST(grid_expand_quad)
{
	Hex h1 = {5, 27};
	Hex h2 = {48, 62};
	Quad quad = {{2, 1}, {125, 62}};
	Grid *g = malloc(sizeof *g);

	grid_initialize(g, &quad);

	grid_set(g, h1, 1);
	grid_set(g, h2, 2);

	quad = (Quad) {{-254, 1}, {125, 126}};
	grid_expand_quad(g, &quad);

	_d(grid_get(g, h1));
	//=> 1
	_d(grid_get(g, h2));
	//=> 2

	Hex h3 = {-200, 120};
	grid_set(g, h3, 3);

	quad = (Quad) {{-510, -254}, {1021, 126}};
	grid_expand_quad(g, &quad);

	_d(grid_get(g, h1));
	//=> 1
	_d(grid_get(g, h2));
	//=> 2
	_d(grid_get(g, h3));
	//=> 3

	grid_terminate(g);
	free(g);
}
