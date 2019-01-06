#include <assert.h>
#include <math.h>
#include "cluster.h"

#define CLUSTER_CAPACITY_GROWTH_FACTOR 0.07

Cluster *cluster_create(Area capacity_area)
{
	assert(!area_empty(capacity_area));

	Cluster *c = malloc(sizeof *c);

	c->capacity_area = capacity_area;
	c->area = AREA_EMPTY;
	c->data_count = 0;
	c->data = calloc(area_capacity(capacity_area), sizeof *c->data);

	return c;
}

void cluster_destroy(Cluster *c)
{
	free(c->data);
	free(c);
}

void *cluster_get(Cluster *c, Hex h)
{
	if (area_contains(c->area, h)) {
		return c->data[area_index(c->capacity_area, h)];
	} else {
		return NULL;
	}
}

void cluster_set(Cluster *c, Hex h, void *datum)
{
	Area area = c->area;
	Area capacity_area = c->capacity_area;

	assert(datum != NULL);

	if (area_contains(area, h)) {
		i32 i = area_index(capacity_area, h);
		if (c->data_count == 0) {
			c->area.min = h;
			c->area.max = h;
		}

		if (c->data[i] == NULL) {
			++c->data_count;
		}
		c->data[i] = datum;

		return;
	}

	if (c->data_count == 0) {
		c->area.min = h;
		c->area.max = h;
	} else {
		c->area = area_expand(c->area, h, 0.0);
	}

	if (area_contains(capacity_area, h)) {
		i32 i = area_index(capacity_area, h);
		c->data[i] = datum;
		++c->data_count;

		return;
	}

	Area new_capacity_area = area_expand(capacity_area, h, CLUSTER_CAPACITY_GROWTH_FACTOR);
	void **data = calloc(area_capacity(new_capacity_area), sizeof *data);
	void **old_data = c->data;

	for (i32 r = area.min.r; r <= area.max.r; ++r) {
		for (i32 q = area.min.q; q <= area.max.q; ++q) {
			Hex h = {.q = q, .r = r};
			i32 i = area_index(new_capacity_area, h);
			i32 old_i = area_index(capacity_area, h);
			data[i] = old_data[old_i];
		}
	}

	free(old_data);

	data[area_index(new_capacity_area, h)] = datum;
	++c->data_count;
	c->data = data;
	c->capacity_area = new_capacity_area;
}

// Returns 1 if item existed and was removed, 0 if not
u8 cluster_remove(Cluster *c, Hex h)
{
	if (c->data_count == 0 || !area_contains(c->area, h)) {
		return 0;
	}

	i32 i = area_index(c->capacity_area, h);
	if (c->data[i] == NULL) {
		return 0;
	}

	c->data[i] = NULL;
	--c->data_count;

	if (c->data_count == 0) {
		c->area = AREA_EMPTY;
		return 1;
	}
	
	Area a = c->area;

	if (area_on_edge(a, h)) {
		Area new_area = AREA_EMPTY;

		for (i32 r = a.min.r; r <= a.max.r; ++r) {
			for (i32 q = a.min.q; q <= a.max.q; ++q) {
				Hex h = {.q = q, .r = r};
				i32 i = area_index(c->capacity_area, h);

				if (c->data[i] != NULL) {
					new_area.max.r = r;
					if (r < new_area.min.r) {
						new_area.min.r = r;
					}
					if (q > new_area.max.q) {
						new_area.max.q = q;
					}
					if (q < new_area.min.q) {
						new_area.min.q = q;
					}
				}
			}
		}

		c->area = new_area;
	}

	return 1;
}

void cluster_move(Cluster *c, Hex move_by)
{
	c->area = area_move(c->area, move_by);
	c->capacity_area = area_move(c->capacity_area, move_by);
}
