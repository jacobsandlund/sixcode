#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "grid.h"

typedef struct {
	Quad new_capacity_quad;
	Grid *g;
	BitArray *new_set;
	void **new_data;
} GridAddEachContext;

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

i8 grid_has(Grid *g, Hex h)
{
	return quad_contains(g->quad, h) &&
		bit_array_has(g->set, quad_index(g->capacity_quad, h));
}

static void grid_add_each_fn(void *context, Hex h, void *data)
{
	GridAddEachContext *c = context;
	Grid *g = c->g;
	i32 i = quad_index(c->new_capacity_quad, h);
	i32 old_i = quad_index(g->capacity_quad, h);
	bit_array_copy_set(c->new_set, i, g->set, old_i);

	if (g->with_data) {
		c->new_data[i] = data;
	}
}

void grid_add(Grid *g, Hex h, void *data)
{
	if (quad_contains(g->quad, h)) {
		i32 i = quad_index(g->capacity_quad, h);

		if (!bit_array_has(g->set, i)) {
			bit_array_set(g->set, i);
			++g->set_count;
		}

		if (g->with_data) {
			g->data[i] = data;
		}

		return;
	}

	Quad new_quad = quad_expand(g->quad, h);

	if (quad_contains(g->capacity_quad, h)) {
		g->quad = new_quad;

		i32 i = quad_index(g->capacity_quad, h);
		bit_array_set(g->set, i);
		++g->set_count;

		if (g->with_data) {
			g->data[i] = data;
		}

		return;
	}

	Quad new_capacity_quad = quad_capacity_quad(new_quad, g->extra_capacity, g->extra_capacity_growth_factor);
	i32 new_capacity = quad_capacity(new_capacity_quad);
	GridAddEachContext ctx = {
		.new_capacity_quad = new_capacity_quad,
		.g = g,
		.new_set = bit_array_create(new_capacity),
	};

	if (g->with_data) {
		ctx.new_data = calloc(new_capacity, sizeof *g->data);
	}

	grid_each(g, g->quad, &ctx, grid_add_each_fn);

	bit_array_destroy(g->set);

	i32 i = quad_index(new_capacity_quad, h);
	bit_array_set(ctx.new_set, i);
	++g->set_count;

	if (g->with_data) {
		free(g->data);
		ctx.new_data[i] = data;
		g->data = ctx.new_data;
	}

	g->set = ctx.new_set;
	g->quad = new_quad;
	g->capacity_quad = new_capacity_quad;
}

static void grid_remove_each_fn(void *context, Hex h, void *data)
{
	Grid *g = context;
	(void) data;

	g->quad = quad_expand(g->quad, h);
}

// Returns 1 if item existed and was removed, 0 if not
i8 grid_remove(Grid *g, Hex h)
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
		Quad old_quad = g->quad;
		g->quad = QUAD_EMPTY;

		grid_each(g, old_quad, g, grid_remove_each_fn);
	}

	return 1;
}

void grid_move(Grid *g, Hex move_by)
{
	g->quad = quad_move(g->quad, move_by);
	g->capacity_quad = quad_move(g->capacity_quad, move_by);
}

void grid_each(Grid *g, Quad quad, void *context, GridEach each_fn)
{
	BitArray *set = g->set;
	Quad cap_quad = g->capacity_quad;
	i32 r_spacing = ((cap_quad.max.c - cap_quad.min.c) >> 1) + 1;
	i32 min_c_odd = quad.min.c & 1;
	i32 cap_min_c_odd = cap_quad.min.c & 1;
	i32 max_c_odd = quad.max.c & 1;

	for (i32 r = quad.min.r; r <= quad.max.r; ++r) {
		i32 c_odd = r & 1;

		i32 min_c = quad.min.c + (c_odd ^ min_c_odd);
		i32 max_c = quad.max.c - (c_odd ^ max_c_odd);
		i32 cap_min_c = cap_quad.min.c + (c_odd ^ cap_min_c_odd);

		i32 min_c_div_2 = min_c >> 1;
		i32 max_c_div_2 = max_c >> 1;
		i32 cap_min_c_div_2 = cap_min_c >> 1;

		i32 diff_min_r = r - cap_quad.min.r;
		i32 i_offset = diff_min_r * r_spacing - cap_min_c_div_2;

		i32 min_i = min_c_div_2 + i_offset;
		i32 max_i = max_c_div_2 + i_offset;
		i32 i = min_i;

		while (i <= max_i) {
			i32 i_div_64 = i >> BIT_ARRAY_SHIFT;
			u64 set_bits = set[i_div_64];

			if (set_bits) {
				u64 bit = (u64) 1 << (i & BIT_ARRAY_MASK);

				if ((set_bits & bit) != (u64) 0) {
					i32 c_div_2 = i - i_offset;
					i32 c = c_div_2 * 2 + c_odd;
					Hex h = {.c = c, .r = r};
					void *data = g->with_data ? g->data[i] : NULL;
					each_fn(context, h, data);
				}

				++i;
			} else {
				i = (i_div_64 + 1) << BIT_ARRAY_SHIFT;
			}
		}
	}
}
