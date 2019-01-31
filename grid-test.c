#include <assert.h>
#include "test.h"
#include "bit-array.c"
#include "grid.c"
#include "hex.c"
#include "quad.c"

#define _hx(h) _dd(h.c, h.r)

TEST(grid_basics)
{
	Hex h1 = {5, 27};
	Hex h2 = {48, 62};
	Quad quad = {{0, 0}, {63, 63}};
	Grid *g = malloc(sizeof *g);

	grid_initialize(g, &quad);

	grid_set(g, h1, 1);
	grid_set(g, h2, 2);

	_d(grid_has(g, h1));
	//=> 1
	_d(grid_get(g, h1));
	//=> 1
	_d(grid_has(g, h2));
	//=> 1
	_d(grid_get(g, h2));
	//=> 2

	grid_clear(g, h2);

	_d(grid_has(g, h1));
	//=> 1
	_d(grid_has(g, h2));
	//=> 0
	_d(grid_get(g, h2));
	//=> 0

	grid_terminate(g);

	free(g);
}

//typedef struct {
//	Grid *result_g;
//	i32 each_called_count;
//} TestGridEachContext;
//
//static void test_grid_each_fn(void *context, Hex h, void *data)
//{
//	TestGridEachContext *c = context;
//	grid_set(c->result_g, h, data);
//	++c->each_called_count;
//}
//
//TEST(grid_each)
//{
//	TestData td = {.d = 1};
//	Hex h1 = {.c = 2, .r = 4};
//	Hex h2 = {.c = -3, .r = -1};
//	Hex h3 = {.c = 12, .r = -20};
//	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_WITH_DATA);
//	Grid *result_g = grid_create(HEX_ZERO, 0.0, GRID_WITH_DATA);
//	TestGridEachContext context = {
//		.result_g = result_g,
//		.each_called_count = 0,
//		.each_called_count_with_data = 0,
//	};
//
//	grid_set(g, h1, &td);
//	grid_set(g, h2, &td);
//	grid_set(g, h3, &td);
//
//	grid_each(g, g->quad, &context, test_grid_each_fn);
//
//	_d(context.each_called_count);
//	//=> 3
//	_d(context.each_called_count_with_data);
//	//=> 3
//	_d(result_g->set_count);
//	//=> 3
//	_d(grid_get(result_g, h1) == &td);
//	//=> 1
//	_d(grid_get(result_g, h2) == &td);
//	//=> 1
//
//	grid_destroy(g);
//	grid_destroy(result_g);
//}
//
//TEST(grid_each_no_data)
//{
//	Hex h1 = {.c = 2, .r = 4};
//	Hex h2 = {.c = -3, .r = 1};
//	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
//	Grid *result_g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
//	TestGridEachContext context = {
//		.result_g = result_g,
//		.each_called_count = 0,
//	};
//
//	grid_set(g, h1, NULL);
//	grid_set(g, h2, NULL);
//
//	grid_each(g, g->quad, &context, test_grid_each_fn);
//
//	_d(context.each_called_count);
//	//=> 2
//	_d(result_g->set_count);
//	//=> 2
//	_d(grid_has(result_g, h1));
//	//=> 1
//	_d(grid_has(result_g, h2));
//	//=> 1
//
//	grid_destroy(g);
//	grid_destroy(result_g);
//}
//
//TEST(grid_each_different_quad)
//{
//	Hex h1 = {.c = 2, .r = 4};
//	Hex h2 = {.c = -3, .r = 1};
//	Hex h3 = {.c = 12, .r = -20};
//	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
//	Grid *result_g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
//	TestGridEachContext context = {
//		.result_g = result_g,
//		.each_called_count = 0,
//	};
//
//	grid_set(g, h1, NULL);
//	grid_set(g, h2, NULL);
//	grid_set(g, h3, NULL);
//
//	_qd(g->capacity_quad);
//	//=> (-3, -20), (12, 4)
//	_qd(g->quad);
//	//=> (-3, -20), (12, 4)
//
//	Quad quad = g->quad;
//
//	grid_each(g, quad, &context, test_grid_each_fn);
//	_d(context.each_called_count); context.each_called_count = 0;
//	//=> 3
//
//	quad.min.c = -2;
//	grid_each(g, quad, &context, test_grid_each_fn);
//	_d(context.each_called_count); context.each_called_count = 0;
//	//=> 2
//
//	quad.min.c = 2; quad.min.r = -2;
//	grid_each(g, quad, &context, test_grid_each_fn);
//	_d(context.each_called_count); context.each_called_count = 0;
//	//=> 1
//
//	quad.min.c = -3; quad.min.r = -20;
//	quad.max.c = 11;
//	grid_each(g, quad, &context, test_grid_each_fn);
//	_d(context.each_called_count); context.each_called_count = 0;
//	//=> 2
//
//	quad.max.c = 1;
//	grid_each(g, quad, &context, test_grid_each_fn);
//	_d(context.each_called_count); context.each_called_count = 0;
//	//=> 1
//
//	quad.max.c = 1; quad.max.r = 0;
//	grid_each(g, quad, &context, test_grid_each_fn);
//	_d(context.each_called_count); context.each_called_count = 0;
//	//=> 0
//
//	grid_destroy(g);
//	grid_destroy(result_g);
//}
