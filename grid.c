#include "grid.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

const ivec2 GRID_BLOCK_SIZE = {128, 64};

// Extra room for border
void grid_quad_to_styles_quad(Quad *styles_quad, Quad *quad)
{
	ivec2 size_delta = {+2, +1};
	quad_resize_by_delta(styles_quad, quad, size_delta);
	quad_block_align(styles_quad, styles_quad, GRID_BLOCK_SIZE);
}

static void grid_set_quads(Grid *g, SizeQuad *storage_quad, Quad *quad)
{
	ivec2 size_delta = {-2, -1};
	grid_quad_to_styles_quad(&g->styles_quad, quad);
	quad_resize_by_delta(&g->quad, &g->styles_quad, size_delta);
	quad_to_storage_size_quad(storage_quad, &g->styles_quad);
}

void grid_initialize(Grid *g, Quad *quad)
{
	grid_set_quads(g, &g->storage_quad, quad);
	int capacity = size_quad_capacity(&g->storage_quad);
	g->styles = calloc(capacity, sizeof *g->styles);
}

void grid_terminate(Grid *g)
{
	free(g->styles);
}

static int grid_index(Grid *g, ivec2 h)
{
	assert(quad_contains(&g->quad, h));
	int diff_min_x = (h.x >> 1) - g->storage_quad.min.x;
	int diff_min_y = h.y - g->storage_quad.min.y;
	return diff_min_x + diff_min_y * g->storage_quad.size.x;
}

u8 grid_get(Grid *g, ivec2 h)
{
	assert(quad_contains(&g->quad, h));
	return g->styles[grid_index(g, h)];
}

void grid_set(Grid *g, ivec2 h, u8 style)
{
	assert(quad_contains(&g->quad, h));
	int i = grid_index(g, h);
	g->styles[i] = style;
}

void grid_clear(Grid *g, ivec2 h)
{
	assert(quad_contains(&g->quad, h));
	int i = grid_index(g, h);
	g->styles[i] = 0;
}

void grid_expand_quad(Grid *g, Quad *quad)
{
	assert(quad_contains_quad(quad, &g->quad));

	SizeQuad new_storage_quad;
	grid_set_quads(g, &new_storage_quad, quad);

	int capacity = size_quad_capacity(&new_storage_quad);
	u8 *new_styles = calloc(capacity, sizeof *new_styles);

	ivec2 min = ivec2_sub(g->storage_quad.min, new_storage_quad.min);
	ivec2 old_size = g->storage_quad.size;
	int new_size_x = new_storage_quad.size.x;

	for (int y = 0; y < old_size.y; y++) {
		int dest_i = (y + min.y) * new_size_x + min.x;
		u8 *dest = &new_styles[dest_i];
		const u8 *src = &g->styles[y * old_size.x];

		memcpy(dest, src, old_size.x);
	}

	g->storage_quad = new_storage_quad;

	free(g->styles);
	g->styles = new_styles;
}
