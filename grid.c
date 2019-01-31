#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "grid.h"

void grid_initialize(Grid *g, Quad *quad)
{
	assert(quad_is_simple(quad));
	storage_quad_from_quad(&g->storage_quad, quad);
	i32 capacity = storage_quad_capacity(&g->storage_quad);
	g->quad = *quad;
	g->set = calloc(bit_array_word_capacity(capacity), sizeof *g->set);
	g->styles = calloc(capacity, sizeof *g->styles);
}

void grid_terminate(Grid *g)
{
	free(g->set);
	free(g->styles);
}

//void grid_expand_quad(Grid *g, Quad *quad)
//{
//	assert(quad_is_simple(quad) && quad_contains_quad(quad, g-quad)));
//	storage_quad_from_quad(&g->storage_quad, quad);
//	i32 capacity = storage_quad_capacity(&g->storage_quad);
//	g->quad = *quad;
//	g->set = calloc(bit_array_word_capacity(capacity), sizeof *g->set);
//	g->styles = calloc(capacity, sizeof *g->styles);
//}
//
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

i8 grid_has(Grid *g, Hex h)
{
	assert(quad_contains(&g->quad, h));
	return bit_array_has(g->set, grid_index(g, h));
}

void grid_set(Grid *g, Hex h, u8 style)
{
	assert(quad_contains(&g->quad, h));
	i32 i = grid_index(g, h);
	bit_array_set(g->set, i);
	g->styles[i] = style;
}

void grid_clear(Grid *g, Hex h)
{
	assert(quad_contains(&g->quad, h));
	i32 i = grid_index(g, h);
	bit_array_clear(g->set, i);
	g->styles[i] = 0;
}

void grid_each(Grid *g, Quad *quad, void *context, GridEach each_fn)
{
	BitArray *set = g->set;
	Quad *grid_quad = &g->quad;
	i32 storage_size_c = g->storage_quad.size.c;
	assert(quad_contains_quad(grid_quad, quad));

	for (i32 r = quad->min.r; r <= quad->max.r; ++r) {
		i32 diff_min_r = r - grid_quad->min.r;
		i32 c_to_i_offset = diff_min_r * storage_size_c - grid_quad->min.c;

		i32 min_i = quad->min.c + c_to_i_offset;
		i32 max_i = quad->max.c + c_to_i_offset;
		i32 i = min_i;

		while (i <= max_i) {
			i32 i_div_64 = i >> BIT_ARRAY_SHIFT;
			u64 set_bits = set[i_div_64];

			if (set_bits) {
				u64 bit = (u64) 1 << (i & BIT_ARRAY_MASK);

				if ((set_bits & bit) != (u64) 0) {
					i32 c = i - c_to_i_offset;
					Hex h = {.c = c, .r = r};
					u8 style = g->styles[i];
					each_fn(context, h, style);
				}

				++i;
			} else {
				i = (i_div_64 + 1) << BIT_ARRAY_SHIFT;
			}
		}
	}
}
