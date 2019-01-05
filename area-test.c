#include "test.h"
#include "area.c"
#include "group.c"
#include "hex.c"

#define _hx(h) _dd(h.q, h.r)

typedef struct {
	i32 d;
} TestData;

TEST(area_create_and_destroy)
{
	Area *a = area_create(6);
	_d(a->count);
	//=> 0
	_d(a->capacity);
	//=> 6
	_d(a->groups != NULL);
	//=> 1

	area_destroy(a);
}

TEST(area_add_group)
{
	Group *g1 = group_create(hex_zero, hex_zero);
	Group *g2 = group_create(hex_zero, hex_zero);
	Area *a = area_create(1);

	area_add_group(a, g1);

	_d(a->count);
	//=> 1
	_d(a->capacity);
	//=> 1
	_d(a->groups[0] == g1);
	//=> 1

	area_add_group(a, g2);

	_d(a->count);
	//=> 2
	_d(a->capacity);
	//=> 2
	_d(a->groups[0] == g1);
	//=> 1
	_d(a->groups[1] == g1);
	//=> 0

	area_add_group(a, g2);

	_d(a->count);
	//=> 3
	_d(a->capacity);
	//=> 4

	group_destroy(g1);
	group_destroy(g2);
	area_destroy(a);
}

TEST(area_nearest_group)
{
	TestData td = {.d = 123};
	Hex h1 = {.q = -2, .r = -5};
	Hex h2 = {.q = 3, .r = 4};
	Hex h3 = {.q = 0, .r = 3}; // closer to h2
	Group *g1 = group_create(hex_zero, hex_zero);
	Group *g2 = group_create(hex_zero, hex_zero);
	Area *a = area_create(2);

	group_set(g1, h1, &td);
	group_set(g2, h2, &td);
	area_add_group(a, g1);
	area_add_group(a, g2);

	_d(area_nearest_group(a, h1) == g1);
	//=> 1
	_d(area_nearest_group(a, h2) == g2);
	//=> 1
	_d(area_nearest_group(a, h3) == g2);
	//=> 1

	group_destroy(g1);
	group_destroy(g2);
	area_destroy(a);
}

void test_area_each_group_fn(void *context, Group *g)
{
	Area *result_a = (Area *) context;
	area_add_group(result_a, g);
}

TEST(area_each_group)
{
	TestData td = {.d = 123};
	Hex h1 = {.q = -2, .r = -5};
	Hex h2 = {.q = 3, .r = 4};
	Group *g1 = group_create(hex_zero, hex_zero);
	Group *g2 = group_create(hex_zero, hex_zero);
	Area *a = area_create(2);
	Area *result_a = area_create(2);

	group_set(g1, h1, &td);
	group_set(g2, h2, &td);
	area_add_group(a, g1);
	area_add_group(a, g2);

	area_each_group(a, result_a, test_area_each_group_fn);

	_d(area_nearest_group(result_a, h1) == g1);
	//=> 1
	_d(area_nearest_group(result_a, h2) == g2);
	//=> 1

	group_destroy(g1);
	group_destroy(g2);
	area_destroy(a);
	area_destroy(result_a);
}
