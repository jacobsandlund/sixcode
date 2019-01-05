#include <assert.h>
#include <stdlib.h>
#include "group.h"

Group *group_create(u32 capacity)
{
	Group *g = malloc(sizeof *g);

	assert(capacity > 0);

	g->count = 0;
	g->capacity = capacity;
	g->clusters = calloc(capacity, sizeof *g->clusters);

	return g;
}

void group_destroy(Group *g)
{
	free(g->clusters);
	free(g);
}

void group_add_cluster(Group *g, Cluster *c)
{
	u32 count = g->count;
	u32 capacity = g->capacity;
	Cluster **clusters = g->clusters;

	if (count + 1 > capacity) {
		Cluster **new_clusters;
		capacity *= 2;
		new_clusters = malloc(capacity * sizeof *new_clusters);

		for (u32 i = 0; i < count; ++i) {
			new_clusters[i] = clusters[i];
		}

		free(clusters);
		clusters = g->clusters = new_clusters;
		g->capacity = capacity;
	}

	clusters[count] = c;
	g->count = count + 1;
}

Cluster *group_nearest_cluster(Group *g, Hex h)
{
	Cluster **clusters = g->clusters;
	u32 count = g->count;
	u32 shortest_distance = I32_MAX;
	Cluster *nearest_cluster = NULL;

	for (u32 i = 0; i < count; ++i) {
		Cluster *c = clusters[i];
		u32 distance = cluster_distance(c, h);
		if (distance < shortest_distance) {
			shortest_distance = distance;
			nearest_cluster = c;
		}
	}

	return nearest_cluster;
}

void group_each_cluster(Group *g, void *context, GroupEach each_fn)
{
	Cluster **clusters = g->clusters;
	u32 count = g->count;

	for (u32 i = 0; i < count; ++i) {
		each_fn(context, clusters[i]);
	}
}
