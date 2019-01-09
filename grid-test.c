#include <assert.h>
#include "test.h"
#include "grid.c"
#include "hex.c"
#include "quad.c"

#define _hx(h) _dd(h.q, h.r)
#define _qd(qd) _("(%d, %d), (%d, %d)\n", qd.min.q, qd.min.r, qd.max.q, qd.max.r);

typedef struct {
	i32 d;
} TestData;

i8 test_compare_data(void *a_, void *b_)
{
	TestData *a = a_;
	TestData *b = b_;
	return a->d == b->d ? 0 : 1;
}

TEST(grid_create_and_destroy)
{
	Quad capacity_quad = {
		.min = {.q = 1, .r = 1},
		.max = {.q = 8, .r = 4},
	};
	Grid *g = grid_create(capacity_quad);

	_d(g->data_count);
	//=> 0
	_qd(g->quad);
	//=> (1073741824, 1073741824), (-1073741824, -1073741824)
	_qd(g->capacity_quad);
	//=> (1, 1), (8, 4)
	_d(quad_capacity(g->capacity_quad));
	//=> 32
	_d(g->data[0]);
	//=> 0
	_d(g->data[quad_capacity(g->capacity_quad) - 1]);
	//=> 0
	_d(g->set[0]);
	//=> 0

	grid_destroy(g);
}

TEST(grid_get)
{
	Hex h2;
	TestData *result;
	TestData td = {.d = 42};
	Hex h1 = {.q = 1, .r = 3};
	Quad capacity_quad = {
		.min = {.q = -4, .r = 2},
		.max = {.q = 7, .r = 3},
	};
	Quad quad = {
		.min = {.q = 1, .r = 2},
		.max = capacity_quad.max,
	};
	Grid *g = grid_create(capacity_quad);
	i32 i = quad_index(capacity_quad, h1);

	g->quad = quad;

	_d(i);
	//=> 17
	g->data[i] = (void *) &td;

	result = (TestData *) grid_get(g, h1);
	assert(result != NULL);

	_d(result->d);
	//=> 42

	h2.q = capacity_quad.min.q - 1; h2.r = capacity_quad.min.r;
	_d(grid_get(g, h2) == NULL);
	//=> 1

	h2.q = h1.q; h2.r = capacity_quad.max.r + 1;
	_d(grid_get(g, h2) == NULL);
	//=> 1

	// The `i` calculation would be in bounds, but shouldn't return anything
	h2.q = capacity_quad.max.q + 1; h2.r = capacity_quad.min.r;
	g->data[h2.q - capacity_quad.min.q] = (void *) &td;
	_d(grid_get(g, h2) == NULL);
	//=> 1

	// ... same
	h2.q = quad.min.q - 1; h2.r = quad.min.r;
	g->data[h2.q - capacity_quad.min.q] = (void *) &td;
	_d(grid_get(g, h2) == NULL);
	//=> 1

	grid_destroy(g);
}

TEST(grid_set)
{
	TestData td1 = {.d = 42};
	TestData td2 = {.d = -1234};
	Hex h1 = {.q = 0, .r = 0};
	Hex h2 = {.q = 1, .r = 0};
	Quad capacity_quad = {
		.min = {.q = -2, .r = -1},
		.max = {.q = 0, .r = 1},
	};
	Grid *g = grid_create(capacity_quad);

	_d(quad_capacity(g->capacity_quad));
	//=> 9
	_qd(g->quad);
	//=> (1073741824, 1073741824), (-1073741824, -1073741824)

	// No expansion needed
	grid_set(g, h1, &td1);
	_d(quad_capacity(g->capacity_quad));
	//=> 9
	_d(g->data_count);
	//=> 1
	_d(grid_get(g, h1) == &td1);
	//=> 1
	_d(((TestData *) grid_get(g, h1))->d);
	//=> 42
	_qd(g->quad);
	//=> (0, 0), (0, 0)

	// Expansion needed
	grid_set(g, h2, &td2);
	_d(quad_capacity(g->capacity_quad));
	//=> 12
	_d(g->data_count);
	//=> 2
	_qd(g->quad);
	//=> (0, 0), (1, 0)
	_qd(g->capacity_quad);
	//=> (-2, -1), (1, 1)
	_d(((TestData *) grid_get(g, h1))->d);
	//=> 42
	_d(((TestData *) grid_get(g, h2))->d);
	//=> -1234

	// Keep the same count if overwriting
	grid_set(g, h1, &td2);
	_d(g->data_count);
	//=> 2
	_d(((TestData *) grid_get(g, h1))->d);
	//=> -1234

	// Expand with extra capacity factor
	h1.q = 7; h1.r = 1;
	grid_set(g, h1, &td1);
	_d(quad_capacity(g->capacity_quad));
	//=> 33
	_qd(g->quad);
	//=> (0, 0), (7, 1)
	_qd(g->capacity_quad);
	//=> (-2, -1), (8, 1)
	_d(((TestData *) grid_get(g, h1))->d);
	//=> 42
	_d(((TestData *) grid_get(g, h2))->d);
	//=> -1234

	// Change quad but no expansion needed
	h2.q = 8; h2.r = 0;
	grid_set(g, h2, &td2);
	_d(quad_capacity(g->capacity_quad));
	//=> 33
	_qd(g->quad);
	//=> (0, 0), (8, 1)
	_qd(g->capacity_quad);
	//=> (-2, -1), (8, 1)
	_d(((TestData *) grid_get(g, h1))->d);
	//=> 42
	_d(((TestData *) grid_get(g, h2))->d);
	//=> -1234

	// Quad/count get set correctly when count = 0, and expanding
	h2.q = -1; h2.r = -1;
	capacity_quad.min.r = 0;
	grid_destroy(g);
	g = grid_create(capacity_quad);
	grid_set(g, h2, &td2);
	_qd(g->quad);
	//=> (-1, -1), (-1, -1)
	_d(g->data_count);
	//=> 1

	grid_destroy(g);
}

TEST(grid_remove)
{
	TestData td = {.d = 42};
	Hex h1 = {.q = 0, .r = 1};
	Hex h2 = {.q = -1, .r = 0};
	Hex h3 = {.q = 0, .r = 0};
	Quad capacity_quad = {
		.min = {.q = -2, .r = -1},
		.max = {.q = 0, .r = 1},
	};
	Grid *g = grid_create(capacity_quad);

	_d(g->data_count);
	//=> 0

	// Empty grid
	_d(grid_remove(g, h1));
	//=> 0

	grid_set(g, h1, &td);
	_d(g->data_count);
	//=> 1
	_d(grid_get(g, h1) == &td);
	//=> 1
	_qd(g->quad);
	//=> (0, 1), (0, 1)

	// Outside quad
	_d(grid_remove(g, h2));
	//=> 0
	_d(g->data_count);
	//=> 1

	// Removing the only existing hex resets quad to empty
	_d(grid_remove(g, h1));
	//=> 1
	_d(g->data_count);
	//=> 0
	_d(grid_get(g, h1) == NULL);
	//=> 1
	_qd(g->quad);
	//=> (1073741824, 1073741824), (-1073741824, -1073741824)

	grid_set(g, h1, &td);
	grid_set(g, h2, &td);
	_qd(g->quad);
	//=> (-1, 0), (0, 1)

	// Hex isn't set (but is in quad)
	_d(grid_remove(g, h3));
	//=> 0

	// Reset quad by searching
	grid_set(g, h3, &td);
	_d(grid_remove(g, h3));
	//=> 1
	_qd(g->quad);
	//=> (-1, 0), (0, 1)

	// ... Again
	_d(grid_remove(g, h2));
	//=> 1
	_qd(g->quad);
	//=> (0, 1), (0, 1)

	grid_destroy(g);
}

TEST(grid_move)
{
	TestData td = {.d = 1};
	Quad capacity_quad = {
		.min = {.q = -2, .r = 5},
		.max = {.q = 4, .r = 8},
	};
	Hex h = {.q = 3, .r = 6};
	Hex move_by = {.q = -2, .r = -4};
	Grid *g = grid_create(capacity_quad);

	grid_set(g, h, &td);

	_qd(g->quad);
	//=> (3, 6), (3, 6)
	_qd(g->capacity_quad);
	//=> (-2, 5), (4, 8)

	grid_move(g, move_by);

	_qd(g->quad);
	//=> (1, 2), (1, 2)
	_qd(g->capacity_quad);
	//=> (-4, 1), (2, 4)

	_d(grid_get(g, h) == NULL);
	//=> 1

	_d(grid_get(g, hex_add(h, move_by)) == &td);
	//=> 1

	grid_destroy(g);
}

void test_grid_each_fn(void *context, Hex h, void *datum)
{
	Grid *result_g = (Grid *) context;
	grid_set(result_g, h, datum);
}

TEST(grid_each)
{
	TestData td = {.d = 1};
	Hex h1 = {.q = 2, .r = 4};
	Hex h2 = {.q = -3, .r = 0};
	Grid *g = grid_create(QUAD_ZERO);
	Grid *result_g = grid_create(QUAD_ZERO);

	grid_set(g, h1, &td);
	grid_set(g, h2, &td);

	grid_each(g, result_g, test_grid_each_fn);

	_d(result_g->data_count);
	//=> 2
	_d(grid_get(result_g, h1) == &td);
	//=> 1
	_d(grid_get(result_g, h2) == &td);
	//=> 1
}
