#include <assert.h>
#include "test.h"
#include "grid.c"
#include "hex.c"
#include "quad.c"

#define _hx(h) _dd(h.c, h.r)

TEST(grid_basics)
{
	Hex h1 = {5, 27};
	Hex h2 = {48, 62};
	Quad quad = {{0, 0}, {127, 63}};
	Grid *g = malloc(sizeof *g);

	grid_initialize(g, &quad);

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
	Quad quad = {{0, 0}, {127, 63}};
	Grid *g = malloc(sizeof *g);

	grid_initialize(g, &quad);

	grid_set(g, h1, 1);
	grid_set(g, h2, 2);

	quad = (Quad) {{-256, 0}, {127, 127}};
	grid_expand_quad(g, &quad);

	_d(grid_get(g, h1));
	//=> 1
	_d(grid_get(g, h2));
	//=> 2

	Hex h3 = {-200, 120};
	grid_set(g, h3, 3);

	quad = (Quad) {{-512, -256}, {1023, 127}};
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
