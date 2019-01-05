#include "test.h"
#include "group.c"
#include "cluster.c"
#include "hex.c"

#define _hx(h) _dd(h.q, h.r)

typedef struct {
	i32 d;
} TestData;

TEST(group_create_and_destroy)
{
	Group *g = group_create(6);
	_d(g->count);
	//=> 0
	_d(g->capacity);
	//=> 6
	_d(g->clusters != NULL);
	//=> 1

	group_destroy(g);
}

TEST(group_add_cluster)
{
	Cluster *c1 = cluster_create(hex_zero, hex_zero);
	Cluster *c2 = cluster_create(hex_zero, hex_zero);
	Group *g = group_create(1);

	group_add_cluster(g, c1);

	_d(g->count);
	//=> 1
	_d(g->capacity);
	//=> 1
	_d(g->clusters[0] == c1);
	//=> 1

	group_add_cluster(g, c2);

	_d(g->count);
	//=> 2
	_d(g->capacity);
	//=> 2
	_d(g->clusters[0] == c1);
	//=> 1
	_d(g->clusters[1] == c1);
	//=> 0

	group_add_cluster(g, c2);

	_d(g->count);
	//=> 3
	_d(g->capacity);
	//=> 4

	cluster_destroy(c1);
	cluster_destroy(c2);
	group_destroy(g);
}

TEST(group_nearest_cluster)
{
	TestData td = {.d = 123};
	Hex h1 = {.q = -2, .r = -5};
	Hex h2 = {.q = 3, .r = 4};
	Hex h3 = {.q = 0, .r = 3}; // closer to h2
	Cluster *c1 = cluster_create(hex_zero, hex_zero);
	Cluster *c2 = cluster_create(hex_zero, hex_zero);
	Group *g = group_create(2);

	cluster_set(c1, h1, &td);
	cluster_set(c2, h2, &td);
	group_add_cluster(g, c1);
	group_add_cluster(g, c2);

	_d(group_nearest_cluster(g, h1) == c1);
	//=> 1
	_d(group_nearest_cluster(g, h2) == c2);
	//=> 1
	_d(group_nearest_cluster(g, h3) == c2);
	//=> 1

	cluster_destroy(c1);
	cluster_destroy(c2);
	group_destroy(g);
}

void test_group_each_cluster_fn(void *context, Cluster *c)
{
	Group *result_a = (Group *) context;
	group_add_cluster(result_a, c);
}

TEST(group_each_cluster)
{
	TestData td = {.d = 123};
	Hex h1 = {.q = -2, .r = -5};
	Hex h2 = {.q = 3, .r = 4};
	Cluster *c1 = cluster_create(hex_zero, hex_zero);
	Cluster *c2 = cluster_create(hex_zero, hex_zero);
	Group *g = group_create(2);
	Group *result_a = group_create(2);

	cluster_set(c1, h1, &td);
	cluster_set(c2, h2, &td);
	group_add_cluster(g, c1);
	group_add_cluster(g, c2);

	group_each_cluster(g, result_a, test_group_each_cluster_fn);

	_d(group_nearest_cluster(result_a, h1) == c1);
	//=> 1
	_d(group_nearest_cluster(result_a, h2) == c2);
	//=> 1

	cluster_destroy(c1);
	cluster_destroy(c2);
	group_destroy(g);
	group_destroy(result_a);
}
