#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "grid.h"

Grid *grid_create(Hex extra_capacity, f64 extra_capacity_growth_factor, i8 with_data)
{
	Grid *g = malloc(sizeof *g);

	g->quad = QUAD_EMPTY;
	g->capacity_quad = QUAD_EMPTY;
	g->extra_capacity = extra_capacity;
	g->extra_capacity_growth_factor = extra_capacity_growth_factor;
	g->set_count = 0;
	g->set = NULL;
	g->data = NULL;
	g->with_data = with_data;

	return g;
}

void grid_destroy(Grid *g)
{
	grid_clear(g);
	free(g);
}

void grid_clear(Grid *g)
{
	bit_array_destroy(g->set);
	free(g->data);
	g->set = NULL;
	g->data = NULL;
}

void *grid_get(Grid *g, Hex h)
{
	assert(g->with_data);

	if (quad_contains(g->quad, h)) {
		return g->data[quad_index(g->capacity_quad, h)];
	} else {
		return NULL;
	}
}

u8 grid_has(Grid *g, Hex h)
{
	return quad_contains(g->quad, h) &&
		bit_array_has(g->set, quad_index(g->capacity_quad, h));
}

void grid_add(Grid *g, Hex h, void *datum)
{
	Quad quad = g->quad;
	Quad capacity_quad = g->capacity_quad;

	if (quad_contains(quad, h)) {
		i32 i = quad_index(capacity_quad, h);

		if (!bit_array_has(g->set, i)) {
			bit_array_set(g->set, i);
			++g->set_count;
		}

		if (g->with_data) {
			g->data[i] = datum;
		}

		return;
	}

	g->quad = quad_expand(g->quad, h);

	if (quad_contains(capacity_quad, h)) {
		i32 i = quad_index(capacity_quad, h);
		bit_array_set(g->set, i);
		++g->set_count;

		if (g->with_data) {
			g->data[i] = datum;
		}

		return;
	}

	Quad new_capacity_quad = quad_capacity_quad(g->quad, g->extra_capacity, g->extra_capacity_growth_factor);
	u32 new_capacity = quad_capacity(new_capacity_quad);
	BitArray *set = bit_array_create(new_capacity);
	BitArray *old_set = g->set;

	for (i32 r = quad.min.r; r <= quad.max.r; ++r) {
		for (i32 q = quad.min.q; q <= quad.max.q; ++q) {
			Hex h = {.q = q, .r = r};
			i32 i = quad_index(new_capacity_quad, h);
			i32 old_i = quad_index(capacity_quad, h);
			bit_array_copy_set(set, i, old_set, old_i);
		}
	}

	bit_array_destroy(old_set);

	i32 i = quad_index(new_capacity_quad, h);
	bit_array_set(set, i);
	++g->set_count;
	g->set = set;
	g->capacity_quad = new_capacity_quad;

	if (g->with_data) {
		void **data = calloc(new_capacity, sizeof *data);
		void **old_data = g->data;

		for (i32 r = quad.min.r; r <= quad.max.r; ++r) {
			for (i32 q = quad.min.q; q <= quad.max.q; ++q) {
				Hex h = {.q = q, .r = r};
				i32 i = quad_index(new_capacity_quad, h);
				i32 old_i = quad_index(capacity_quad, h);
				data[i] = old_data[old_i];
			}
		}

		free(old_data);
		g->data = data;
		data[i] = datum;
	}
}

// Returns 1 if item existed and was removed, 0 if not
u8 grid_remove(Grid *g, Hex h)
{
	if (g->set_count == 0 || !quad_contains(g->quad, h)) {
		return 0;
	}

	i32 i = quad_index(g->capacity_quad, h);
	if (!bit_array_has(g->set, i)) {
		return 0;
	}

	bit_array_clear(g->set, i);
	--g->set_count;

	if (g->with_data) {
		g->data[i] = NULL;
	}

	if (g->set_count == 0) {
		g->quad = QUAD_EMPTY;
		return 1;
	}
	
	Quad quad = g->quad;

	if (quad_on_edge(quad, h)) {
		Quad new_quad = QUAD_EMPTY;

		for (i32 r = quad.min.r; r <= quad.max.r; ++r) {
			for (i32 q = quad.min.q; q <= quad.max.q; ++q) {
				Hex h = {.q = q, .r = r};
				i32 i = quad_index(g->capacity_quad, h);

				if (bit_array_has(g->set, i)) {
					new_quad = quad_expand(new_quad, h);
				}
			}
		}

		g->quad = new_quad;
	}

	return 1;
}

void grid_move(Grid *g, Hex move_by)
{
	g->quad = quad_move(g->quad, move_by);
	g->capacity_quad = quad_move(g->capacity_quad, move_by);
}

void grid_each(Grid *g, void *context, GridEach each_fn)
{
	Quad quad = g->quad;

	for (i32 r = quad.min.r; r <= quad.max.r; ++r) {
		for (i32 q = quad.min.q; q <= quad.max.q; ++q) {
			Hex h = {.q = q, .r = r};
			i32 i = quad_index(g->capacity_quad, h);
			void *datum = g->with_data ? g->data[i] : NULL;

			if (bit_array_has(g->set, i)) {
				each_fn(context, h, datum);
			}
		}
	}
}
