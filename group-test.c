#include "test.h"
#include "group.c"
#include "cluster.c"
#include "area.c"
#include "hex.c"

#define _hx(h) _dd(h.q, h.r)

typedef struct {
	i32 d;
} TestData;

TEST(group_create_and_destroy)
{
	Group *g = group_create(6);
	_d(g->cluster_count);
	//=> 0
	_d(g->cluster_capacity);
	//=> 6
	_d(g->clusters != NULL);
	//=> 1

	group_destroy(g);
}

TEST(group_get)
{
	// TODO
}

TEST(group_set)
{
	// TODO
}

TEST(group_remove)
{
	// TODO
}

TEST(group_hex_count)
{
	// TODO
}

TEST(group_each)
{
	// TODO
}
