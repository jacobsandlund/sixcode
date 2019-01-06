#include <assert.h>
#include <stdlib.h>
#include "group.h"

Group *group_create(u32 cluster_capacity)
{
	Group *g = malloc(sizeof *g);

	assert(cluster_capacity > 0);

	g->cluster_count = 0;
	g->cluster_capacity = cluster_capacity;
	g->clusters = calloc(cluster_capacity, sizeof *g->clusters);

	// TODO
	g->clusters[0] = cluster_create(AREA_ZERO);

	return g;
}

void group_destroy(Group *g)
{
	free(g->clusters);
	free(g);
}

void *group_get(Group *g, Hex h)
{
	return cluster_get(g->clusters[0], h);
}

void group_set(Group *g, Hex h, void *datum)
{
	cluster_set(g->clusters[0], h, datum);
}

u8 group_remove(Group *g, Hex h)
{
	return cluster_remove(g->clusters[0], h);
}

u32 group_hex_count(Group *g)
{
	Area a = g->clusters[0]->area;
	Area capacity_area = g->clusters[0]->capacity_area;
	void **data = g->clusters[0]->data;
	u32 hex_count = 0;

	for (i32 r = a.min.r; r <= a.max.r; ++r) {
		for (i32 q = a.min.q; q <= a.max.q; ++q) {
			Hex h = {.q = q, .r = r};
			i32 i = area_index(capacity_area, h);
			void *datum = data[i];

			if (datum != NULL) {
				++hex_count;
			}
		}
	}

	return hex_count;
}

void group_each(Group *g, void *context, GroupEach each_fn)
{
	Area a = g->clusters[0]->area;
	Area capacity_area = g->clusters[0]->capacity_area;
	void **data = g->clusters[0]->data;

	for (i32 r = a.min.r; r <= a.max.r; ++r) {
		for (i32 q = a.min.q; q <= a.max.q; ++q) {
			Hex h = {.q = q, .r = r};
			i32 i = area_index(capacity_area, h);
			void *datum = data[i];

			if (datum != NULL) {
				each_fn(context, h, datum);
			}
		}
	}
}
