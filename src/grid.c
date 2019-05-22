#include "grid.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define GRID_HALF_SIZE 2048

void grid_initialize(Grid *g)
{
	g->quad = (Quad) {
		{-GRID_HALF_SIZE + 1, -GRID_HALF_SIZE + 1},
		{GRID_HALF_SIZE - 2, GRID_HALF_SIZE - 2},
	};
	g->styles_quad = (Quad) {
		{-GRID_HALF_SIZE, -GRID_HALF_SIZE},
		{GRID_HALF_SIZE - 1, GRID_HALF_SIZE - 1},
	};
	quad_to_size_quad(&g->size_quad, &g->styles_quad);
	i64 capacity = size_quad_capacity(&g->size_quad);
	g->styles = calloc(capacity, sizeof *g->styles);
}

void grid_terminate(Grid *g)
{
	free(g->styles);
}

static i64 grid_index(Grid *g, int2 h)
{
	assert(quad_contains(&g->quad, h));
	i64 diff_min_x = h.x - g->size_quad.min.x;
	i64 diff_min_y = h.y - g->size_quad.min.y;
	return diff_min_x + diff_min_y * g->size_quad.size.x;
}

u8 grid_get(Grid *g, int2 h)
{
	assert(quad_contains(&g->quad, h));
	return g->styles[grid_index(g, h)];
}

void grid_set(Grid *g, int2 h, u8 style)
{
	assert(quad_contains(&g->quad, h));
	i64 i = grid_index(g, h);
	g->styles[i] = style;
}

void grid_clear(Grid *g, int2 h)
{
	assert(quad_contains(&g->quad, h));
	i64 i = grid_index(g, h);
	g->styles[i] = 0;
}
