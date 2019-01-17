#include <assert.h>
#include "test.h"
#include "bit-array.c"
#include "grid.c"
#include "hex.c"
#include "quad.c"

#define _hx(h) _dd(h.c, h.r)
#define _qd(q) _("(%d, %d), (%d, %d)\n", q.min.c, q.min.r, q.max.c, q.max.r);

typedef struct {
	i32 d;
} TestData;

TEST(grid_create_and_destroy)
{
	Hex extra_capacity = {.c = 6, .r = 2};
	f64 growth_factor = 0.15;
	Grid *g = grid_create(extra_capacity, growth_factor, GRID_WITH_DATA);

	_d(quad_empty(g->quad));
	//=> 1
	_d(quad_empty(g->capacity_quad));
	//=> 1
	_hx(g->extra_capacity);
	//=> 6, 2
	_g(g->extra_capacity_growth_factor);
	//=> 0.15
	_d(g->set_count);
	//=> 0
	_d(g->set == NULL);
	//=> 1
	_d(g->data == NULL);
	//=> 1
	_d(g->with_data);
	//=> 1

	grid_destroy(g);
}

TEST(grid_clear)
{
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_WITH_DATA);

	g->set = malloc(sizeof *g->set);
	g->data = malloc(sizeof *g->data);

	grid_clear(g);

	_d(g->set == NULL);
	//=> 1
	_d(g->data == NULL);
	//=> 1

	grid_destroy(g);
}

TEST(grid_basic_add_get_has)
{
	TestData td = {.d = 555};
	Hex h = {.c = 3, .r = -5};
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_WITH_DATA);

	grid_add(g, h, &td);

	_d(grid_has(g, h));
	//=> 1
	assert(grid_get(g, h) != NULL);
	_d(grid_get(g, h) == &td);
	//=> 1
	_d(((TestData *) grid_get(g, h))->d);
	//=> 555

	grid_destroy(g);
}

TEST(grid_get)
{
	TestData td = {.d = 42};
	Hex h = {.c = 1, .r = 3};
	Hex extra_capacity = {.c = 4, .r = 2};
	Grid *g = grid_create(extra_capacity, 0.0, GRID_WITH_DATA);

	grid_add(g, h, &td);

	_d(grid_get(g, h) == &td);
	//=> 1

	// The `i` calculation would be in bounds, but shouldn't return anything
	_qd(g->quad);
	//=> (1, 3), (1, 3)
	_qd(g->capacity_quad);
	//=> (-3, 1), (5, 5)
	h.c = g->capacity_quad.max.c + 2; h.r = g->capacity_quad.min.r;
	g->data[h.c - g->capacity_quad.min.c] = (void *) &td;
	_d(grid_get(g, h) == NULL);
	//=> 1

	grid_destroy(g);
}

TEST(grid_add)
{
	TestData td1 = {.d = 42};
	TestData td2 = {.d = -1234};
	Hex h1 = {.c = -4, .r = 2};
	Hex h2 = {.c = -1, .r = 1};
	Hex extra_capacity = {.c = 3, .r = 2};
	f64 growth_factor = 0.2;
	Grid *g = grid_create(extra_capacity, growth_factor, GRID_WITH_DATA);

	// Expanding from empty
	grid_add(g, h1, &td1);
	_qd(g->quad);
	//=> (-4, 2), (-4, 2)
	_qd(g->capacity_quad);
	//=> (-7, 0), (-1, 4)
	_d(grid_has(g, h1));
	//=> 1
	_d(grid_get(g, h1) == &td1);
	//=> 1

	// No expansion needed
	grid_add(g, h2, &td2);
	_qd(g->capacity_quad);
	//=> (-7, 0), (-1, 4)
	_d(g->set_count);
	//=> 2
	_d(grid_has(g, h2));
	//=> 1
	_d(grid_get(g, h2) == &td2);
	//=> 1

	// Expansion needed
	_qd(g->quad);
	//=> (-4, 1), (-1, 2)
	_hx(h1);
	//=> -4, 2
	_hx(h2);
	//=> -1, 1
	grid_remove(g, h1);
	_d(grid_has(g, h2));
	//=> 1

	h1.c = 3; h1.r = -1;
	grid_add(g, h1, &td1);
	_qd(g->quad);
	//=> (-1, -1), (3, 1)
	_qd(g->capacity_quad);
	//=> (-5, -4), (7, 4)
	_d(g->set_count);
	//=> 2
	_d(grid_has(g, h1));
	//=> 1
	_d(grid_has(g, h2));
	//=> 1
	_d(grid_get(g, h1) == &td1);
	//=> 1
	_d(grid_get(g, h2) == &td2);
	//=> 1

	// Overwriting existing data
	grid_add(g, h2, &td1);
	_d(g->set_count);
	//=> 2
	_d(((TestData *) grid_get(g, h2))->d);
	//=> 42

	grid_destroy(g);
}

TEST(grid_add_no_data)
{
	Hex h1 = {.c = 2, .r = 4};
	Hex h2 = {.c = -2, .r = 6};
	Hex extra_capacity = {.c = 3, .r = 1};
	f64 growth_factor = 0.15;
	Grid *g = grid_create(extra_capacity, growth_factor, GRID_NO_DATA);

	// Empty grid
	_d(grid_has(g, h1));
	//=> 0
	grid_add(g, h1, NULL);
	_d(grid_has(g, h1));
	//=> 1

	// Expand
	_qd(g->capacity_quad);
	//=> (-1, 3), (5, 5)
	grid_add(g, h2, NULL);
	_qd(g->capacity_quad);
	//=> (-6, 3), (6, 7)
	_d(grid_has(g, h1));
	//=> 1
	_d(grid_has(g, h2));
	//=> 1

	// Add again
	grid_add(g, h1, NULL);
	_d(grid_has(g, h1));
	//=> 1

	grid_destroy(g);
}

TEST(grid_remove)
{
	TestData td = {.d = 42};
	Hex h1 = {.c = 0, .r = 2};
	Hex h2 = {.c = -2, .r = 0};
	Hex h3 = {.c = -2, .r = 2};
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_WITH_DATA);

	// Empty grid
	_d(grid_remove(g, h1));
	//=> 0

	grid_add(g, h1, &td);
	_d(g->set_count);
	//=> 1
	_d(grid_get(g, h1) == &td);
	//=> 1
	_qd(g->quad);
	//=> (0, 2), (0, 2)

	// Outside quad
	_d(grid_remove(g, h2));
	//=> 0
	_d(g->set_count);
	//=> 1

	// Removing the only existing hex resets quad to empty
	_d(grid_remove(g, h1));
	//=> 1
	_d(g->set_count);
	//=> 0
	_d(grid_get(g, h1) == NULL);
	//=> 1
	_d(!grid_has(g, h1));
	//=> 1
	_d(quad_empty(g->quad));
	//=> 1

	grid_add(g, h1, &td);
	grid_add(g, h2, &td);
	_qd(g->quad);
	//=> (-2, 0), (0, 2)

	// Hex isn't set (but is in quad)
	_d(grid_remove(g, h3));
	//=> 0

	// Reset quad by searching
	grid_add(g, h3, &td);
	_d(grid_remove(g, h3));
	//=> 1
	_qd(g->quad);
	//=> (-2, 0), (0, 2)

	// ... Again
	_d(grid_remove(g, h2));
	//=> 1
	_qd(g->quad);
	//=> (0, 2), (0, 2)

	grid_destroy(g);
}

TEST(grid_move)
{
	Hex h = {.c = 2, .r = 6};
	Hex move_by = {.c = -2, .r = -4};
	Hex extra_capacity = {.c = 3, .r = 2};
	Grid *g = grid_create(extra_capacity, 0.0, GRID_NO_DATA);

	grid_add(g, h, NULL);

	_qd(g->quad);
	//=> (2, 6), (2, 6)
	_qd(g->capacity_quad);
	//=> (-1, 4), (5, 8)

	grid_move(g, move_by);

	_qd(g->quad);
	//=> (0, 2), (0, 2)
	_qd(g->capacity_quad);
	//=> (-3, 0), (3, 4)

	_d(!grid_has(g, h));
	//=> 1

	_d(grid_has(g, hex_add(h, move_by)));
	//=> 1

	grid_destroy(g);
}

typedef struct {
	Grid *result_g;
	i32 each_called_count;
	i32 each_called_count_with_data;
} TestGridEachContext;

static void test_grid_each_fn(void *context, Hex h, void *data)
{
	TestGridEachContext *c = context;
	grid_add(c->result_g, h, data);
	++c->each_called_count;

	if (data != NULL) {
		++c->each_called_count_with_data;
	}
}

TEST(grid_each)
{
	TestData td = {.d = 1};
	Hex h1 = {.c = 2, .r = 4};
	Hex h2 = {.c = -3, .r = -1};
	Hex h3 = {.c = 12, .r = -20};
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_WITH_DATA);
	Grid *result_g = grid_create(HEX_ZERO, 0.0, GRID_WITH_DATA);
	TestGridEachContext context = {
		.result_g = result_g,
		.each_called_count = 0,
		.each_called_count_with_data = 0,
	};

	grid_add(g, h1, &td);
	grid_add(g, h2, &td);
	grid_add(g, h3, &td);

	grid_each(g, g->quad, &context, test_grid_each_fn);

	_d(context.each_called_count);
	//=> 3
	_d(context.each_called_count_with_data);
	//=> 3
	_d(result_g->set_count);
	//=> 3
	_d(grid_get(result_g, h1) == &td);
	//=> 1
	_d(grid_get(result_g, h2) == &td);
	//=> 1

	grid_destroy(g);
	grid_destroy(result_g);
}

TEST(grid_each_no_data)
{
	Hex h1 = {.c = 2, .r = 4};
	Hex h2 = {.c = -3, .r = 1};
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
	Grid *result_g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
	TestGridEachContext context = {
		.result_g = result_g,
		.each_called_count = 0,
	};

	grid_add(g, h1, NULL);
	grid_add(g, h2, NULL);

	grid_each(g, g->quad, &context, test_grid_each_fn);

	_d(context.each_called_count);
	//=> 2
	_d(result_g->set_count);
	//=> 2
	_d(grid_has(result_g, h1));
	//=> 1
	_d(grid_has(result_g, h2));
	//=> 1

	grid_destroy(g);
	grid_destroy(result_g);
}

TEST(grid_each_different_quad)
{
	Hex h1 = {.c = 2, .r = 4};
	Hex h2 = {.c = -3, .r = 1};
	Hex h3 = {.c = 12, .r = -20};
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
	Grid *result_g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
	TestGridEachContext context = {
		.result_g = result_g,
		.each_called_count = 0,
	};

	grid_add(g, h1, NULL);
	grid_add(g, h2, NULL);
	grid_add(g, h3, NULL);

	_qd(g->capacity_quad);
	//=> (-3, -20), (12, 4)
	_qd(g->quad);
	//=> (-3, -20), (12, 4)

	Quad quad = g->quad;

	grid_each(g, quad, &context, test_grid_each_fn);
	_d(context.each_called_count); context.each_called_count = 0;
	//=> 3

	quad.min.c = -2;
	grid_each(g, quad, &context, test_grid_each_fn);
	_d(context.each_called_count); context.each_called_count = 0;
	//=> 2

	quad.min.c = 2; quad.min.r = -2;
	grid_each(g, quad, &context, test_grid_each_fn);
	_d(context.each_called_count); context.each_called_count = 0;
	//=> 1

	quad.min.c = -3; quad.min.r = -20;
	quad.max.c = 11;
	grid_each(g, quad, &context, test_grid_each_fn);
	_d(context.each_called_count); context.each_called_count = 0;
	//=> 2

	quad.max.c = 1;
	grid_each(g, quad, &context, test_grid_each_fn);
	_d(context.each_called_count); context.each_called_count = 0;
	//=> 1

	quad.max.c = 1; quad.max.r = 0;
	grid_each(g, quad, &context, test_grid_each_fn);
	_d(context.each_called_count); context.each_called_count = 0;
	//=> 0

	grid_destroy(g);
	grid_destroy(result_g);
}
