#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"

const Hex GRID_BLOCK_SIZE = {GRID_BLOCK_SIZE_C, GRID_BLOCK_SIZE_R};

void grid_initialize(Grid *g, Quad *quad)
{
	assert(quad_is_block_aligned(quad, GRID_BLOCK_SIZE));
	storage_quad_from_quad(&g->storage_quad, quad);
	i32 capacity = storage_quad_capacity(&g->storage_quad);
	g->quad = *quad;
	g->styles = calloc(capacity, sizeof *g->styles);
}

void grid_terminate(Grid *g)
{
	free(g->styles);
}

static i32 grid_index(Grid *g, Hex h)
{
	assert(quad_contains(&g->quad, h));
	Hex diff_min = hex_sub(h, g->quad.min);
	return (diff_min.c >> 1) + diff_min.r * g->storage_quad.size.c;
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
	assert(quad_is_block_aligned(quad, GRID_BLOCK_SIZE) && quad_contains_quad(quad, &g->quad));

	StorageQuad new_storage_quad;
	storage_quad_from_quad(&new_storage_quad, quad);

	i32 capacity = storage_quad_capacity(&new_storage_quad);
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

	g->quad = *quad;
	g->storage_quad = new_storage_quad;

	free(g->styles);
	g->styles = new_styles;
}
