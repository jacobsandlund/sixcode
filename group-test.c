#include <assert.h>
#include "test.h"
#include "group.c"
#include "hex.c"

#define _hx(h) _dd(h.q, h.r)

typedef struct {
	i32 d;
} TestData;

i8 test_compare_data(void *a_, void *b_)
{
	TestData *a = a_;
	TestData *b = b_;
	return a->d == b->d ? 0 : 1;
}

TEST(group_create_and_destroy)
{
	Hex capacity_min = {.q = 1, .r = 1};
	Hex capacity_max = {.q = 8, .r = 4};
	Group *g = group_create(capacity_min, capacity_max);

	_d(g->count);
	//=> 0
	_d(g->capacity);
	//=> 32
	_d(g->r_spacing);
	//=> 8
	_hx(g->min);
	//=> 1, 1
	_hx(g->max);
	//=> 8, 4
	_hx(g->capacity_min);
	//=> 1, 1
	_hx(g->capacity_max);
	//=> 8, 4
	_d(g->data[0]);
	//=> 0
	_d(g->data[g->capacity - 1]);
	//=> 0

	group_destroy(g);
}

TEST(group_get)
{
	Hex h2;
	TestData *result;
	TestData td = {.d = 42};
	Hex h1 = {.q = 1, .r = 3};
	Hex capacity_min = {.q = -4, .r = 2};
	Hex capacity_max = {.q = 7, .r = 3};
	Group *g = group_create(capacity_min, capacity_max);
	i32 i = (h1.q - capacity_min.q) + g->r_spacing * (h1.r - capacity_min.r);

	_d(g->r_spacing);
	//=> 12
	_d(g->capacity);
	//=> 24
	_hx(g->min);
	//=> -4, 2
	_hx(g->max);
	//=> 7, 3

	_d(i);
	//=> 17
	g->data[i] = (void *) &td;

	result = (TestData *) group_get(g, h1);
	assert(result != NULL);

	_d(result->d);
	//=> 42

	h2.q = capacity_min.q - 1; h2.r = capacity_min.r;
	_d(group_get(g, h2) == NULL);
	//=> 1

	h2.q = h1.q; h2.r = capacity_max.r + 1;
	_d(group_get(g, h2) == NULL);
	//=> 1

	// The `i` calculation would be in bounds, but shouldn't return anything
	h2.q = capacity_max.q + 1; h2.r = capacity_min.r;
	g->data[h2.q - capacity_min.q] = (void *) &td;
	_d(group_get(g, h2) == NULL);
	//=> 1

	g->min.q = h1.q;
	h2.q = h1.q - 1; h2.r = capacity_min.r;
	g->data[h2.q - capacity_min.q] = (void *) &td;
	_d(group_get(g, h2) == NULL);
	//=> 1

	group_destroy(g);
}

TEST(group_distance)
{
	Group g = {
		.min = {.q = -4, .r = 2},
		.max = {.q = 7, .r = 3},
	};
	Hex h = {.q = -4, .r = 3};

	_d(group_distance(&g, h));
	//=> 0

	h.q = -5;
	_d(group_distance(&g, h));
	//=> 1

	h.q = 11;
	_d(group_distance(&g, h));
	//=> 4

	h.q = 0; h.r = -1;
	_d(group_distance(&g, h));
	//=> 3

	h.r = 8;
	_d(group_distance(&g, h));
	//=> 5

	h.q = -5; h.r = -1;
	_d(group_distance(&g, h));
	//=> 3

	h.q = 11; h.r = -1;
	_d(group_distance(&g, h));
	//=> 4

	h.q = 11; h.r = 8;
	_d(group_distance(&g, h));
	//=> 5
}

TEST(group_set)
{
	TestData td1 = {.d = 42};
	TestData td2 = {.d = -1234};
	Hex h1 = {.q = 0, .r = 0};
	Hex h2 = {.q = 1, .r = 0};
	Hex capacity_min = {.q = -2, .r = -1};
	Hex capacity_max = {.q = 0, .r = 1};
	Group *g = group_create(capacity_min, capacity_max);

	_hx(g->min);
	//=> -2, -1
	_hx(g->max);
	//=> 0, 1

	// No expansion needed
	_d(group_set(g, h1, &td1));
	//=> 0
	_d(g->count);
	//=> 1
	_d(group_get(g, h1) == &td1);
	//=> 1
	_d(((TestData *) group_get(g, h1))->d);
	//=> 42
	_hx(g->min);
	//=> 0, 0
	_hx(g->max);
	//=> 0, 0

	// Expansion needed
	_d(group_set(g, h2, &td2));
	//=> 3
	_d(g->count);
	//=> 2
	_hx(g->min);
	//=> 0, 0
	_hx(g->max);
	//=> 1, 0
	_hx(g->capacity_max);
	//=> 1, 1
	_d(g->r_spacing);
	//=> 4
	_d(((TestData *) group_get(g, h1))->d);
	//=> 42
	_d(((TestData *) group_get(g, h2))->d);
	//=> -1234

	// Keep the same count if overwriting
	_d(group_set(g, h1, &td2));
	//=> 0
	_d(g->count);
	//=> 2
	_d(((TestData *) group_get(g, h1))->d);
	//=> -1234

	// Expand with extra capacity factor
	h1.q = 7; h1.r = 1;
	_d(group_set(g, h1, &td1));
	//=> 21
	_hx(g->max);
	//=> 7, 1
	_hx(g->capacity_max);
	//=> 8, 1
	_d(((TestData *) group_get(g, h1))->d);
	//=> 42
	_d(((TestData *) group_get(g, h2))->d);
	//=> -1234

	// Change min/max but no expansion needed
	h2.q = 8; h2.r = 0;
	_d(group_set(g, h2, &td2));
	//=> 0
	_hx(g->max);
	//=> 8, 1
	_hx(g->capacity_max);
	//=> 8, 1
	_d(((TestData *) group_get(g, h1))->d);
	//=> 42
	_d(((TestData *) group_get(g, h2))->d);
	//=> -1234

	// Min/max/count get set correctly when count = 0, and expanding
	h2.q = -1; h2.r = -1;
	capacity_min.r = 0;
	group_destroy(g);
	g = group_create(capacity_min, capacity_max);
	_d(group_set(g, h2, &td2));
	//=> 3
	_hx(g->min);
	//=> -1, -1
	_hx(g->max);
	//=> -1, -1
	_d(g->count);
	//=> 1

	group_destroy(g);
}

TEST(group_remove)
{
	TestData td = {.d = 42};
	Hex h1 = {.q = 0, .r = 1};
	Hex h2 = {.q = -1, .r = 0};
	Hex h3 = {.q = 0, .r = 0};
	Hex capacity_min = {.q = -2, .r = -1};
	Hex capacity_max = {.q = 0, .r = 1};
	Group *g = group_create(capacity_min, capacity_max);

	_d(g->count);
	//=> 0

	// Empty group
	_d(group_remove(g, h1));
	//=> 0

	group_set(g, h1, &td);
	_d(g->count);
	//=> 1
	_d(group_get(g, h1) == &td);
	//=> 1
	_hx(g->min);
	//=> 0, 1
	_hx(g->max);
	//=> 0, 1

	// Outside min/max
	_d(group_remove(g, h2));
	//=> 0
	_d(g->count);
	//=> 1

	// Removing the only existing hex resets min/max to capacity
	_d(group_remove(g, h1));
	//=> 1
	_d(g->count);
	//=> 0
	_d(group_get(g, h1) == NULL);
	//=> 1
	_hx(g->min);
	//=> -2, -1
	_hx(g->max);
	//=> 0, 1

	group_set(g, h1, &td);
	group_set(g, h2, &td);
	_hx(g->min);
	//=> -1, 0
	_hx(g->max);
	//=> 0, 1

	// Hex isn't set (but is in min/max)
	_d(group_remove(g, h3));
	//=> 0

	// Reset min/max by searching
	group_set(g, h3, &td);
	_d(group_remove(g, h3));
	//=> 1
	_hx(g->min);
	//=> -1, 0
	_hx(g->max);
	//=> 0, 1

	// ... Again
	_d(group_remove(g, h2));
	//=> 1
	_hx(g->min);
	//=> 0, 1
	_hx(g->max);
	//=> 0, 1

	group_destroy(g);
}

TEST(group_move)
{
	TestData td = {.d = 1};
	Hex capacity_min = {.q = -2, .r = 5};
	Hex capacity_max = {.q = 4, .r = 8};
	Hex h = {.q = 3, .r = 6};
	Hex move_by = {.q = -2, .r = -4};
	Group *g = group_create(capacity_min, capacity_max);

	group_set(g, h, &td);

	_hx(g->min);
	//=> 3, 6
	_hx(g->max);
	//=> 3, 6

	group_move(g, move_by);

	_hx(g->min);
	//=> 1, 2
	_hx(g->max);
	//=> 1, 2
	_hx(g->capacity_min);
	//=> -4, 1
	_hx(g->capacity_max);
	//=> 2, 4

	_d(group_get(g, h) == NULL);
	//=> 1

	h.q += move_by.q;
	h.r += move_by.r;
	_d(group_get(g, h) == &td);
	//=> 1

	group_destroy(g);
}

TEST(group_equal)
{
	TestData td1 = {.d = 42};
	TestData td2 = {.d = -1234};
	Hex h1 = {.q = -1, .r = -1};
	Hex h2 = {.q = 1, .r = 1};
	Hex h3 = {.q = 3, .r = -1};
	Hex move_by = {.q = 4, .r = -2};
	Hex a_capacity_min = {.q = -2, .r = -1};
	Hex b_capacity_min = {.q = -5, .r = 0};
	Hex capacity_max = {.q = 2, .r = 1};
	Group *a = group_create(a_capacity_min, capacity_max);
	Group *b = group_create(b_capacity_min, capacity_max);

	// Empty groups
	_d(group_equal(a, b, test_compare_data));
	//=> 1

	group_set(a, h1, &td1);
	group_set(b, h1, &td1);

	// Equal (count = 1)
	_d(group_equal(a, b, test_compare_data));
	//=> 1

	group_set(a, h2, &td2);

	// Different count
	_d(group_equal(a, b, test_compare_data));
	//=> 0

	group_set(b, h2, &td2);

	// Equal (count = 2)
	_d(group_equal(a, b, &test_compare_data));
	//=> 1

	group_move(b, move_by);

	// Equal regardless of translation
	_d(group_equal(a, b, &test_compare_data));
	//=> 1

	group_move(b, hex_sub(hex_zero, move_by));

	group_remove(b, h2);
	group_set(b, h3, &td2);

	// Different size (max - min)
	_d(group_equal(a, b, &test_compare_data));
	//=> 0

	group_remove(b, h3);
	group_set(b, h2, &td1);

	// Different value
	_d(group_equal(a, b, &test_compare_data));
	//=> 0

	group_remove(b, h1);
	group_remove(b, h2);
	h1.q = 1;  // swap h1, h2
	h2.q = -1;
	group_set(b, h1, &td1);
	group_set(b, h2, &td2);
	_hx(b->min);
	//=> -1, -1
	_hx(b->max);
	//=> 1, 1

	// Different NULL or != NULL
	_d(group_equal(a, b, &test_compare_data));
	//=> 0

	group_destroy(a);
	group_destroy(b);
}

void test_group_each_fn(void *context, Hex h, void *datum)
{
	Group *result_g = (Group *) context;

	group_set(result_g, h, datum);
}

TEST(group_each)
{
	TestData td1 = {.d = 42};
	TestData td2 = {.d = -1234};
	Hex h1 = {.q = -1, .r = -1};
	Hex h2 = {.q = 1, .r = 1};
	Group *g = group_create(h1, h2);
	Group *result_g = group_create(hex_zero, hex_zero);
	
	group_set(g, h1, &td1);
	group_set(g, h2, &td2);

	group_each(g, result_g, test_group_each_fn);

	_d(group_get(result_g, h1) == &td1);
	//=> 1
	_d(group_get(result_g, h2) == &td2);
	//=> 1

	group_destroy(g);
	group_destroy(result_g);
}
