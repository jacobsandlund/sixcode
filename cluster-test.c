#include <assert.h>
#include "test.h"
#include "cluster.c"
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

TEST(cluster_create_and_destroy)
{
	Hex capacity_min = {.q = 1, .r = 1};
	Hex capacity_max = {.q = 8, .r = 4};
	Cluster *c = cluster_create(capacity_min, capacity_max);

	_d(c->count);
	//=> 0
	_d(c->capacity);
	//=> 32
	_d(c->r_spacing);
	//=> 8
	_hx(c->min);
	//=> 1, 1
	_hx(c->max);
	//=> 8, 4
	_hx(c->capacity_min);
	//=> 1, 1
	_hx(c->capacity_max);
	//=> 8, 4
	_d(c->data[0]);
	//=> 0
	_d(c->data[c->capacity - 1]);
	//=> 0

	cluster_destroy(c);
}

TEST(cluster_get)
{
	Hex h2;
	TestData *result;
	TestData td = {.d = 42};
	Hex h1 = {.q = 1, .r = 3};
	Hex capacity_min = {.q = -4, .r = 2};
	Hex capacity_max = {.q = 7, .r = 3};
	Cluster *c = cluster_create(capacity_min, capacity_max);
	i32 i = (h1.q - capacity_min.q) + c->r_spacing * (h1.r - capacity_min.r);

	_d(c->r_spacing);
	//=> 12
	_d(c->capacity);
	//=> 24
	_hx(c->min);
	//=> -4, 2
	_hx(c->max);
	//=> 7, 3

	_d(i);
	//=> 17
	c->data[i] = (void *) &td;

	result = (TestData *) cluster_get(c, h1);
	assert(result != NULL);

	_d(result->d);
	//=> 42

	h2.q = capacity_min.q - 1; h2.r = capacity_min.r;
	_d(cluster_get(c, h2) == NULL);
	//=> 1

	h2.q = h1.q; h2.r = capacity_max.r + 1;
	_d(cluster_get(c, h2) == NULL);
	//=> 1

	// The `i` calculation would be in bounds, but shouldn't return anything
	h2.q = capacity_max.q + 1; h2.r = capacity_min.r;
	c->data[h2.q - capacity_min.q] = (void *) &td;
	_d(cluster_get(c, h2) == NULL);
	//=> 1

	c->min.q = h1.q;
	h2.q = h1.q - 1; h2.r = capacity_min.r;
	c->data[h2.q - capacity_min.q] = (void *) &td;
	_d(cluster_get(c, h2) == NULL);
	//=> 1

	cluster_destroy(c);
}

TEST(cluster_distance)
{
	Cluster c = {
		.min = {.q = -4, .r = 2},
		.max = {.q = 7, .r = 3},
	};
	Hex h = {.q = -4, .r = 3};

	_d(cluster_distance(&c, h));
	//=> 0

	h.q = -5;
	_d(cluster_distance(&c, h));
	//=> 1

	h.q = 11;
	_d(cluster_distance(&c, h));
	//=> 4

	h.q = 0; h.r = -1;
	_d(cluster_distance(&c, h));
	//=> 3

	h.r = 8;
	_d(cluster_distance(&c, h));
	//=> 5

	h.q = -5; h.r = -1;
	_d(cluster_distance(&c, h));
	//=> 3

	h.q = 11; h.r = -1;
	_d(cluster_distance(&c, h));
	//=> 4

	h.q = 11; h.r = 8;
	_d(cluster_distance(&c, h));
	//=> 5
}

TEST(cluster_set)
{
	TestData td1 = {.d = 42};
	TestData td2 = {.d = -1234};
	Hex h1 = {.q = 0, .r = 0};
	Hex h2 = {.q = 1, .r = 0};
	Hex capacity_min = {.q = -2, .r = -1};
	Hex capacity_max = {.q = 0, .r = 1};
	Cluster *c = cluster_create(capacity_min, capacity_max);

	_hx(c->min);
	//=> -2, -1
	_hx(c->max);
	//=> 0, 1

	// No expansion needed
	_d(cluster_set(c, h1, &td1));
	//=> 0
	_d(c->count);
	//=> 1
	_d(cluster_get(c, h1) == &td1);
	//=> 1
	_d(((TestData *) cluster_get(c, h1))->d);
	//=> 42
	_hx(c->min);
	//=> 0, 0
	_hx(c->max);
	//=> 0, 0

	// Expansion needed
	_d(cluster_set(c, h2, &td2));
	//=> 3
	_d(c->count);
	//=> 2
	_hx(c->min);
	//=> 0, 0
	_hx(c->max);
	//=> 1, 0
	_hx(c->capacity_max);
	//=> 1, 1
	_d(c->r_spacing);
	//=> 4
	_d(((TestData *) cluster_get(c, h1))->d);
	//=> 42
	_d(((TestData *) cluster_get(c, h2))->d);
	//=> -1234

	// Keep the same count if overwriting
	_d(cluster_set(c, h1, &td2));
	//=> 0
	_d(c->count);
	//=> 2
	_d(((TestData *) cluster_get(c, h1))->d);
	//=> -1234

	// Expand with extra capacity factor
	h1.q = 7; h1.r = 1;
	_d(cluster_set(c, h1, &td1));
	//=> 21
	_hx(c->max);
	//=> 7, 1
	_hx(c->capacity_max);
	//=> 8, 1
	_d(((TestData *) cluster_get(c, h1))->d);
	//=> 42
	_d(((TestData *) cluster_get(c, h2))->d);
	//=> -1234

	// Change min/max but no expansion needed
	h2.q = 8; h2.r = 0;
	_d(cluster_set(c, h2, &td2));
	//=> 0
	_hx(c->max);
	//=> 8, 1
	_hx(c->capacity_max);
	//=> 8, 1
	_d(((TestData *) cluster_get(c, h1))->d);
	//=> 42
	_d(((TestData *) cluster_get(c, h2))->d);
	//=> -1234

	// Min/max/count get set correctly when count = 0, and expanding
	h2.q = -1; h2.r = -1;
	capacity_min.r = 0;
	cluster_destroy(c);
	c = cluster_create(capacity_min, capacity_max);
	_d(cluster_set(c, h2, &td2));
	//=> 3
	_hx(c->min);
	//=> -1, -1
	_hx(c->max);
	//=> -1, -1
	_d(c->count);
	//=> 1

	cluster_destroy(c);
}

TEST(cluster_remove)
{
	TestData td = {.d = 42};
	Hex h1 = {.q = 0, .r = 1};
	Hex h2 = {.q = -1, .r = 0};
	Hex h3 = {.q = 0, .r = 0};
	Hex capacity_min = {.q = -2, .r = -1};
	Hex capacity_max = {.q = 0, .r = 1};
	Cluster *c = cluster_create(capacity_min, capacity_max);

	_d(c->count);
	//=> 0

	// Empty cluster
	_d(cluster_remove(c, h1));
	//=> 0

	cluster_set(c, h1, &td);
	_d(c->count);
	//=> 1
	_d(cluster_get(c, h1) == &td);
	//=> 1
	_hx(c->min);
	//=> 0, 1
	_hx(c->max);
	//=> 0, 1

	// Outside min/max
	_d(cluster_remove(c, h2));
	//=> 0
	_d(c->count);
	//=> 1

	// Removing the only existing hex resets min/max to capacity
	_d(cluster_remove(c, h1));
	//=> 1
	_d(c->count);
	//=> 0
	_d(cluster_get(c, h1) == NULL);
	//=> 1
	_hx(c->min);
	//=> -2, -1
	_hx(c->max);
	//=> 0, 1

	cluster_set(c, h1, &td);
	cluster_set(c, h2, &td);
	_hx(c->min);
	//=> -1, 0
	_hx(c->max);
	//=> 0, 1

	// Hex isn't set (but is in min/max)
	_d(cluster_remove(c, h3));
	//=> 0

	// Reset min/max by searching
	cluster_set(c, h3, &td);
	_d(cluster_remove(c, h3));
	//=> 1
	_hx(c->min);
	//=> -1, 0
	_hx(c->max);
	//=> 0, 1

	// ... Again
	_d(cluster_remove(c, h2));
	//=> 1
	_hx(c->min);
	//=> 0, 1
	_hx(c->max);
	//=> 0, 1

	cluster_destroy(c);
}

TEST(cluster_move)
{
	TestData td = {.d = 1};
	Hex capacity_min = {.q = -2, .r = 5};
	Hex capacity_max = {.q = 4, .r = 8};
	Hex h = {.q = 3, .r = 6};
	Hex move_by = {.q = -2, .r = -4};
	Cluster *c = cluster_create(capacity_min, capacity_max);

	cluster_set(c, h, &td);

	_hx(c->min);
	//=> 3, 6
	_hx(c->max);
	//=> 3, 6

	cluster_move(c, move_by);

	_hx(c->min);
	//=> 1, 2
	_hx(c->max);
	//=> 1, 2
	_hx(c->capacity_min);
	//=> -4, 1
	_hx(c->capacity_max);
	//=> 2, 4

	_d(cluster_get(c, h) == NULL);
	//=> 1

	h.q += move_by.q;
	h.r += move_by.r;
	_d(cluster_get(c, h) == &td);
	//=> 1

	cluster_destroy(c);
}

TEST(cluster_equal)
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
	Cluster *a = cluster_create(a_capacity_min, capacity_max);
	Cluster *b = cluster_create(b_capacity_min, capacity_max);

	// Empty clusters
	_d(cluster_equal(a, b, test_compare_data));
	//=> 1

	cluster_set(a, h1, &td1);
	cluster_set(b, h1, &td1);

	// Equal (count = 1)
	_d(cluster_equal(a, b, test_compare_data));
	//=> 1

	cluster_set(a, h2, &td2);

	// Different count
	_d(cluster_equal(a, b, test_compare_data));
	//=> 0

	cluster_set(b, h2, &td2);

	// Equal (count = 2)
	_d(cluster_equal(a, b, &test_compare_data));
	//=> 1

	cluster_move(b, move_by);

	// Equal regardless of translation
	_d(cluster_equal(a, b, &test_compare_data));
	//=> 1

	cluster_move(b, hex_sub(hex_zero, move_by));

	cluster_remove(b, h2);
	cluster_set(b, h3, &td2);

	// Different size (max - min)
	_d(cluster_equal(a, b, &test_compare_data));
	//=> 0

	cluster_remove(b, h3);
	cluster_set(b, h2, &td1);

	// Different value
	_d(cluster_equal(a, b, &test_compare_data));
	//=> 0

	cluster_remove(b, h1);
	cluster_remove(b, h2);
	h1.q = 1;  // swap h1, h2
	h2.q = -1;
	cluster_set(b, h1, &td1);
	cluster_set(b, h2, &td2);
	_hx(b->min);
	//=> -1, -1
	_hx(b->max);
	//=> 1, 1

	// Different NULL or != NULL
	_d(cluster_equal(a, b, &test_compare_data));
	//=> 0

	cluster_destroy(a);
	cluster_destroy(b);
}

void test_cluster_each_fn(void *context, Hex h, void *datum)
{
	Cluster *result_c = (Cluster *) context;

	cluster_set(result_c, h, datum);
}

TEST(cluster_each)
{
	TestData td1 = {.d = 42};
	TestData td2 = {.d = -1234};
	Hex h1 = {.q = -1, .r = -1};
	Hex h2 = {.q = 1, .r = 1};
	Cluster *c = cluster_create(h1, h2);
	Cluster *result_c = cluster_create(hex_zero, hex_zero);
	
	cluster_set(c, h1, &td1);
	cluster_set(c, h2, &td2);

	cluster_each(c, result_c, test_cluster_each_fn);

	_d(cluster_get(result_c, h1) == &td1);
	//=> 1
	_d(cluster_get(result_c, h2) == &td2);
	//=> 1

	cluster_destroy(c);
	cluster_destroy(result_c);
}
