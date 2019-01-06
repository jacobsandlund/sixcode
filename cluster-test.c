#include <assert.h>
#include "test.h"
#include "cluster.c"
#include "hex.c"
#include "area.c"

#define _hx(h) _dd(h.q, h.r)
#define _ar(a) _("(%d, %d), (%d, %d)\n", a.min.q, a.min.r, a.max.q, a.max.r);

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
	Area capacity_area = {
		.min = {.q = 1, .r = 1},
		.max = {.q = 8, .r = 4},
	};
	Cluster *c = cluster_create(capacity_area);

	_d(c->data_count);
	//=> 0
	_ar(c->area);
	//=> (1073741824, 1073741824), (-1073741824, -1073741824)
	_ar(c->capacity_area);
	//=> (1, 1), (8, 4)
	_d(area_capacity(c->capacity_area));
	//=> 32
	_d(c->data[0]);
	//=> 0
	_d(c->data[area_capacity(c->capacity_area) - 1]);
	//=> 0

	cluster_destroy(c);
}

TEST(cluster_get)
{
	Hex h2;
	TestData *result;
	TestData td = {.d = 42};
	Hex h1 = {.q = 1, .r = 3};
	Area capacity_area = {
		.min = {.q = -4, .r = 2},
		.max = {.q = 7, .r = 3},
	};
	Area area = {
		.min = {.q = 1, .r = 2},
		.max = capacity_area.max,
	};
	Cluster *c = cluster_create(capacity_area);
	i32 i = area_index(capacity_area, h1);

	c->area = area;

	_d(i);
	//=> 17
	c->data[i] = (void *) &td;

	result = (TestData *) cluster_get(c, h1);
	assert(result != NULL);

	_d(result->d);
	//=> 42

	h2.q = capacity_area.min.q - 1; h2.r = capacity_area.min.r;
	_d(cluster_get(c, h2) == NULL);
	//=> 1

	h2.q = h1.q; h2.r = capacity_area.max.r + 1;
	_d(cluster_get(c, h2) == NULL);
	//=> 1

	// The `i` calculation would be in bounds, but shouldn't return anything
	h2.q = capacity_area.max.q + 1; h2.r = capacity_area.min.r;
	c->data[h2.q - capacity_area.min.q] = (void *) &td;
	_d(cluster_get(c, h2) == NULL);
	//=> 1

	// ... same
	h2.q = area.min.q - 1; h2.r = area.min.r;
	c->data[h2.q - capacity_area.min.q] = (void *) &td;
	_d(cluster_get(c, h2) == NULL);
	//=> 1

	cluster_destroy(c);
}

TEST(cluster_set)
{
	TestData td1 = {.d = 42};
	TestData td2 = {.d = -1234};
	Hex h1 = {.q = 0, .r = 0};
	Hex h2 = {.q = 1, .r = 0};
	Area capacity_area = {
		.min = {.q = -2, .r = -1},
		.max = {.q = 0, .r = 1},
	};
	Cluster *c = cluster_create(capacity_area);

	_d(area_capacity(c->capacity_area));
	//=> 9
	_ar(c->area);
	//=> (1073741824, 1073741824), (-1073741824, -1073741824)

	// No expansion needed
	cluster_set(c, h1, &td1);
	_d(area_capacity(c->capacity_area));
	//=> 9
	_d(c->data_count);
	//=> 1
	_d(cluster_get(c, h1) == &td1);
	//=> 1
	_d(((TestData *) cluster_get(c, h1))->d);
	//=> 42
	_ar(c->area);
	//=> (0, 0), (0, 0)

	// Expansion needed
	cluster_set(c, h2, &td2);
	_d(area_capacity(c->capacity_area));
	//=> 12
	_d(c->data_count);
	//=> 2
	_ar(c->area);
	//=> (0, 0), (1, 0)
	_ar(c->capacity_area);
	//=> (-2, -1), (1, 1)
	_d(((TestData *) cluster_get(c, h1))->d);
	//=> 42
	_d(((TestData *) cluster_get(c, h2))->d);
	//=> -1234

	// Keep the same count if overwriting
	cluster_set(c, h1, &td2);
	_d(c->data_count);
	//=> 2
	_d(((TestData *) cluster_get(c, h1))->d);
	//=> -1234

	// Expand with extra capacity factor
	h1.q = 7; h1.r = 1;
	cluster_set(c, h1, &td1);
	_d(area_capacity(c->capacity_area));
	//=> 33
	_ar(c->area);
	//=> (0, 0), (7, 1)
	_ar(c->capacity_area);
	//=> (-2, -1), (8, 1)
	_d(((TestData *) cluster_get(c, h1))->d);
	//=> 42
	_d(((TestData *) cluster_get(c, h2))->d);
	//=> -1234

	// Change area but no expansion needed
	h2.q = 8; h2.r = 0;
	cluster_set(c, h2, &td2);
	_d(area_capacity(c->capacity_area));
	//=> 33
	_ar(c->area);
	//=> (0, 0), (8, 1)
	_ar(c->capacity_area);
	//=> (-2, -1), (8, 1)
	_d(((TestData *) cluster_get(c, h1))->d);
	//=> 42
	_d(((TestData *) cluster_get(c, h2))->d);
	//=> -1234

	// Area/count get set correctly when count = 0, and expanding
	h2.q = -1; h2.r = -1;
	capacity_area.min.r = 0;
	cluster_destroy(c);
	c = cluster_create(capacity_area);
	cluster_set(c, h2, &td2);
	_ar(c->area);
	//=> (-1, -1), (-1, -1)
	_d(c->data_count);
	//=> 1

	cluster_destroy(c);
}

TEST(cluster_remove)
{
	TestData td = {.d = 42};
	Hex h1 = {.q = 0, .r = 1};
	Hex h2 = {.q = -1, .r = 0};
	Hex h3 = {.q = 0, .r = 0};
	Area capacity_area = {
		.min = {.q = -2, .r = -1},
		.max = {.q = 0, .r = 1},
	};
	Cluster *c = cluster_create(capacity_area);

	_d(c->data_count);
	//=> 0

	// Empty cluster
	_d(cluster_remove(c, h1));
	//=> 0

	cluster_set(c, h1, &td);
	_d(c->data_count);
	//=> 1
	_d(cluster_get(c, h1) == &td);
	//=> 1
	_ar(c->area);
	//=> (0, 1), (0, 1)

	// Outside area
	_d(cluster_remove(c, h2));
	//=> 0
	_d(c->data_count);
	//=> 1

	// Removing the only existing hex resets area to empty
	_d(cluster_remove(c, h1));
	//=> 1
	_d(c->data_count);
	//=> 0
	_d(cluster_get(c, h1) == NULL);
	//=> 1
	_ar(c->area);
	//=> (1073741824, 1073741824), (-1073741824, -1073741824)

	cluster_set(c, h1, &td);
	cluster_set(c, h2, &td);
	_ar(c->area);
	//=> (-1, 0), (0, 1)

	// Hex isn't set (but is in area)
	_d(cluster_remove(c, h3));
	//=> 0

	// Reset area by searching
	cluster_set(c, h3, &td);
	_d(cluster_remove(c, h3));
	//=> 1
	_ar(c->area);
	//=> (-1, 0), (0, 1)

	// ... Again
	_d(cluster_remove(c, h2));
	//=> 1
	_ar(c->area);
	//=> (0, 1), (0, 1)

	cluster_destroy(c);
}

TEST(cluster_move)
{
	TestData td = {.d = 1};
	Area capacity_area = {
		.min = {.q = -2, .r = 5},
		.max = {.q = 4, .r = 8},
	};
	Hex h = {.q = 3, .r = 6};
	Hex move_by = {.q = -2, .r = -4};
	Cluster *c = cluster_create(capacity_area);

	cluster_set(c, h, &td);

	_ar(c->area);
	//=> (3, 6), (3, 6)
	_ar(c->capacity_area);
	//=> (-2, 5), (4, 8)

	cluster_move(c, move_by);

	_ar(c->area);
	//=> (1, 2), (1, 2)
	_ar(c->capacity_area);
	//=> (-4, 1), (2, 4)

	_d(cluster_get(c, h) == NULL);
	//=> 1

	_d(cluster_get(c, hex_add(h, move_by)) == &td);
	//=> 1

	cluster_destroy(c);
}
