#include <assert.h>
#include "test.h"
#include "grid.c"
#include "hex.c"
#include "quad.c"
#include "bit-array.c"

#define _hx(h) _dd(h.q, h.r)
#define _qd(qd) _("(%d, %d), (%d, %d)\n", qd.min.q, qd.min.r, qd.max.q, qd.max.r);

typedef struct {
	i32 d;
} TestData;

TEST(grid_create_and_destroy)
{
	Hex extra_capacity = {.q = 5, .r = 2};
	f64 growth_factor = 0.15;
	Grid *g = grid_create(extra_capacity, growth_factor, GRID_WITH_DATA);

	_d(quad_empty(g->quad));
	//=> 1
	_d(quad_empty(g->capacity_quad));
	//=> 1
	_hx(g->extra_capacity);
	//=> 5, 2
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
	Hex h = {.q = 2, .r = -5};
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
	Hex h = {.q = 1, .r = 3};
	Hex extra_capacity = {.q = 2, .r = 1};
	Grid *g = grid_create(extra_capacity, 0.0, GRID_WITH_DATA);

	grid_add(g, h, &td);

	_d(grid_get(g, h) == &td);
	//=> 1

	// The `i` calculation would be in bounds, but shouldn't return anything
	_qd(g->quad);
	//=> (1, 3), (1, 3)
	_qd(g->capacity_quad);
	//=> (-1, 2), (3, 4)
	h.q = g->capacity_quad.max.q + 1; h.r = g->capacity_quad.min.r;
	g->data[h.q - g->capacity_quad.min.q] = (void *) &td;
	_d(grid_get(g, h) == NULL);
	//=> 1

	grid_destroy(g);
}

TEST(grid_add)
{
	TestData td1 = {.d = 42};
	TestData td2 = {.d = -1234};
	Hex h1 = {.q = -3, .r = 2};
	Hex h2 = {.q = 0, .r = 1};
	Hex extra_capacity = {.q = 3, .r = 2};
	f64 growth_factor = 0.2;
	Grid *g = grid_create(extra_capacity, growth_factor, GRID_WITH_DATA);

	// Expanding from empty
	grid_add(g, h1, &td1);
	_d(g->quad);
	//=> -3
	_qd(g->capacity_quad);
	//=> (-6, 0), (0, 4)
	_d(grid_has(g, h1));
	//=> 1
	_d(grid_get(g, h1) == &td1);
	//=> 1

	// No expansion needed
	grid_add(g, h2, &td2);
	_qd(g->capacity_quad);
	//=> (-6, 0), (0, 4)
	_d(g->set_count);
	//=> 2
	_d(grid_has(g, h2));
	//=> 1
	_d(grid_get(g, h2) == &td2);
	//=> 1

	// Expansion needed
	grid_remove(g, h1);
	h1.q = 2; h1.r = -1;
	grid_add(g, h1, &td1);
	_qd(g->quad);
	//=> (0, -1), (2, 1)
	_qd(g->capacity_quad);
	//=> (-4, -4), (6, 4)
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
	Hex h1 = {.q = 3, .r = 4};
	Hex h2 = {.q = -1, .r = 6};
	Hex extra_capacity = {.q = 3, .r = 1};
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
	//=> (0, 3), (6, 5)
	grid_add(g, h2, NULL);
	_qd(g->capacity_quad);
	//=> (-5, 3), (7, 7)
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
	Hex h1 = {.q = 0, .r = 1};
	Hex h2 = {.q = -2, .r = 0};
	Hex h3 = {.q = -2, .r = 1};
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
	//=> (0, 1), (0, 1)

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
	//=> (-2, 0), (0, 1)

	// Hex isn't set (but is in quad)
	_d(grid_remove(g, h3));
	//=> 0

	// Reset quad by searching
	grid_add(g, h3, &td);
	_d(grid_remove(g, h3));
	//=> 1
	_qd(g->quad);
	//=> (-2, 0), (0, 1)

	// ... Again
	_d(grid_remove(g, h2));
	//=> 1
	_qd(g->quad);
	//=> (0, 1), (0, 1)

	grid_destroy(g);
}

TEST(grid_move)
{
	Hex h = {.q = 3, .r = 6};
	Hex move_by = {.q = -2, .r = -4};
	Hex extra_capacity = {.q = 3, .r = 2};
	Grid *g = grid_create(extra_capacity, 0.0, GRID_NO_DATA);

	grid_add(g, h, NULL);

	_qd(g->quad);
	//=> (3, 6), (3, 6)
	_qd(g->capacity_quad);
	//=> (0, 4), (6, 8)

	grid_move(g, move_by);

	_qd(g->quad);
	//=> (1, 2), (1, 2)
	_qd(g->capacity_quad);
	//=> (-2, 0), (4, 4)

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

static void test_grid_each_fn(void *context, Hex h, void *datum)
{
	TestGridEachContext *c = (TestGridEachContext *) context;
	grid_add(c->result_g, h, datum);
	++c->each_called_count;

	if (datum != NULL) {
		++c->each_called_count_with_data;
	}
}

TEST(grid_each)
{
	TestData td = {.d = 1};
	Hex h1 = {.q = 2, .r = 4};
	Hex h2 = {.q = -3, .r = 0};
	Hex h3 = {.q = 13, .r = -20};
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

	grid_each(g, &context, test_grid_each_fn);

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
	Hex h1 = {.q = 2, .r = 4};
	Hex h2 = {.q = -3, .r = 0};
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
	Grid *result_g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
	TestGridEachContext context = {
		.result_g = result_g,
		.each_called_count = 0,
	};

	grid_add(g, h1, NULL);
	grid_add(g, h2, NULL);

	grid_each(g, &context, test_grid_each_fn);

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
