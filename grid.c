#include "grid.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

const Hex GRID_BLOCK_SIZE = {128, 64};

// Extra room for border
void grid_quad_to_styles_quad(Quad *styles_quad, Quad *quad)
{
	Hex size_delta = {+2, +1};
	quad_resize_by_delta(styles_quad, quad, size_delta);
	quad_block_align(styles_quad, styles_quad, GRID_BLOCK_SIZE);
}

static void grid_set_quads(Grid *g, SizeQuad *storage_quad, Quad *quad)
{
	Hex size_delta = {-2, -1};
	grid_quad_to_styles_quad(&g->styles_quad, quad);
	quad_resize_by_delta(&g->quad, &g->styles_quad, size_delta);
	quad_to_storage_size_quad(storage_quad, &g->styles_quad);
}

void grid_initialize(Grid *g, Quad *quad)
{
	grid_set_quads(g, &g->storage_quad, quad);
	i32 capacity = size_quad_capacity(&g->storage_quad);
	g->styles = calloc(capacity, sizeof *g->styles);
}

void grid_terminate(Grid *g)
{
	free(g->styles);
}

static i32 grid_index(Grid *g, Hex h)
{
	assert(quad_contains(&g->quad, h));
	i32 diff_min_c = (h.c >> 1) - g->storage_quad.min.c;
	i32 diff_min_r = h.r - g->storage_quad.min.r;
	return diff_min_c + diff_min_r * g->storage_quad.size.c;
}

u8 grid_get(Grid *g, Hex h)
{
	assert(quad_contains(&g->quad, h));
	return g->styles[grid_index(g, h)];
}

void grid_set(Grid *g, Hex h, u8 style)
{
	assert(quad_contains(&g->quad, h));
	i32 i = grid_index(g, h);
	g->styles[i] = style;
}

void grid_clear(Grid *g, Hex h)
{
	assert(quad_contains(&g->quad, h));
	i32 i = grid_index(g, h);
	g->styles[i] = 0;
}

void grid_expand_quad(Grid *g, Quad *quad)
{
	assert(quad_contains_quad(quad, &g->quad));

	SizeQuad new_storage_quad;
	grid_set_quads(g, &new_storage_quad, quad);

	i32 capacity = size_quad_capacity(&new_storage_quad);
	u8 *new_styles = calloc(capacity, sizeof *new_styles);

	Hex min = hex_sub(g->storage_quad.min, new_storage_quad.min);
	Hex old_size = g->storage_quad.size;
	i32 new_size_c = new_storage_quad.size.c;

	for (i32 r = 0; r < old_size.r; ++r) {
		i32 dest_i = (r + min.r) * new_size_c + min.c;
		u8 *dest = &new_styles[dest_i];
		const u8 *src = &g->styles[r * old_size.c];

		memcpy(dest, src, old_size.c);
	}

	g->storage_quad = new_storage_quad;

	free(g->styles);
	g->styles = new_styles;
}
