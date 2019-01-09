#include <assert.h>
#include <math.h>
#include "grid.h"

#define GRID_CAPACITY_GROWTH_FACTOR 0.07

Grid *grid_create(Quad capacity_quad)
{
	assert(!quad_empty(capacity_quad));

	Grid *g = malloc(sizeof *g);

	g->capacity_quad = capacity_quad;
	g->quad = QUAD_EMPTY;
	g->data_count = 0;
	g->data = calloc(quad_capacity(capacity_quad), sizeof *g->data);

	return g;
}

void grid_destroy(Grid *g)
{
	free(g->data);
	free(g);
}

void *grid_get(Grid *g, Hex h)
{
	if (quad_contains(g->quad, h)) {
		return g->data[quad_index(g->capacity_quad, h)];
	} else {
		return NULL;
	}
}

void grid_set(Grid *g, Hex h, void *datum)
{
	Quad quad = g->quad;
	Quad capacity_quad = g->capacity_quad;

	assert(datum != NULL);

	if (quad_contains(quad, h)) {
		i32 i = quad_index(capacity_quad, h);
		if (g->data_count == 0) {
			g->quad.min = h;
			g->quad.max = h;
		}

		if (g->data[i] == NULL) {
			++g->data_count;
		}
		g->data[i] = datum;

		return;
	}

	if (g->data_count == 0) {
		g->quad.min = h;
		g->quad.max = h;
	} else {
		g->quad = quad_expand(g->quad, h, 0.0);
	}

	if (quad_contains(capacity_quad, h)) {
		i32 i = quad_index(capacity_quad, h);
		g->data[i] = datum;
		++g->data_count;

		return;
	}

	Quad new_capacity_quad = quad_expand(capacity_quad, h, GRID_CAPACITY_GROWTH_FACTOR);
	void **data = calloc(quad_capacity(new_capacity_quad), sizeof *data);
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

	data[quad_index(new_capacity_quad, h)] = datum;
	++g->data_count;
	g->data = data;
	g->capacity_quad = new_capacity_quad;
}

// Returns 1 if item existed and was removed, 0 if not
u8 grid_remove(Grid *g, Hex h)
{
	if (g->data_count == 0 || !quad_contains(g->quad, h)) {
		return 0;
	}

	i32 i = quad_index(g->capacity_quad, h);
	if (g->data[i] == NULL) {
		return 0;
	}

	g->data[i] = NULL;
	--g->data_count;

	if (g->data_count == 0) {
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

				if (g->data[i] != NULL) {
					new_quad.max.r = r;
					if (r < new_quad.min.r) {
						new_quad.min.r = r;
					}
					if (q > new_quad.max.q) {
						new_quad.max.q = q;
					}
					if (q < new_quad.min.q) {
						new_quad.min.q = q;
					}
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
	Quad capacity_quad = g->capacity_quad;
	void **data = g->data;

	for (i32 r = quad.min.r; r <= quad.max.r; ++r) {
		for (i32 q = quad.min.q; q <= quad.max.q; ++q) {
			Hex h = {.q = q, .r = r};
			i32 i = quad_index(capacity_quad, h);
			void *datum = data[i];

			if (datum != NULL) {
				each_fn(context, h, datum);
			}
		}
	}
}
