#include <assert.h>
#include <stdlib.h>
#include "area.h"

Area *area_create(u32 capacity)
{
	Area *a = malloc(sizeof *a);

	assert(capacity > 0);

	a->count = 0;
	a->capacity = capacity;
	a->groups = calloc(capacity, sizeof *a->groups);

	return a;
}

void area_destroy(Area *a)
{
	free(a->groups);
	free(a);
}

void area_add_group(Area *a, Group *g)
{
	u32 count = a->count;
	u32 capacity = a->capacity;
	Group **groups = a->groups;

	if (count + 1 > capacity) {
		Group **new_groups;
		capacity *= 2;
		new_groups = malloc(capacity * sizeof *new_groups);

		for (u32 i = 0; i < count; ++i) {
			new_groups[i] = groups[i];
		}

		free(groups);
		groups = a->groups = new_groups;
		a->capacity = capacity;
	}

	groups[count] = g;
	a->count = count + 1;
}

Group *area_nearest_group(Area *a, Hex h)
{
	Group **groups = a->groups;
	u32 count = a->count;
	u32 shortest_distance = I32_MAX;
	Group *nearest_group = NULL;

	for (u32 i = 0; i < count; ++i) {
		Group *g = groups[i];
		u32 distance = group_distance(g, h);
		if (distance < shortest_distance) {
			shortest_distance = distance;
			nearest_group = g;
		}
	}

	return nearest_group;
}

void area_each_group(Area *a, void *context, AreaEach each_fn)
{
	Group **groups = a->groups;
	u32 count = a->count;

	for (u32 i = 0; i < count; ++i) {
		each_fn(context, groups[i]);
	}
}
