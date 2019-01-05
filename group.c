#include <assert.h>
#include <math.h>
#include "group.h"

#define GROUP_CAPACITY_GROWTH_FACTOR 0.07

static const Hex EMPTY_GROUP_MIN = {
	.q = I32_MAX,
	.r = I32_MAX,
};

static const Hex EMPTY_GROUP_MAX = {
	.q = I32_MIN,
	.r = I32_MIN,
};

Group *group_create(Hex capacity_min, Hex capacity_max)
{
	Group *g = malloc(sizeof *g);
	u32 r_spacing = capacity_max.q - capacity_min.q + 1;
	u32 capacity = (capacity_max.r - capacity_min.r + 1) * r_spacing;

	g->count = 0;
	g->capacity = capacity;
	g->r_spacing = r_spacing;
	g->min = capacity_min;
	g->max = capacity_max;
	g->capacity_min = capacity_min;
	g->capacity_max = capacity_max;
	g->data = calloc(capacity, sizeof *g->data);

	return g;
}

void group_destroy(Group *g)
{
	free(g->data);
	free(g);
}

void *group_get(Group *g, Hex h)
{
	Hex min = g->min;
	Hex max = g->max;
	Hex diff_min = hex_sub(h, min);
	Hex diff_max = hex_sub(max, h);

	if (diff_min.q >= 0 && diff_min.r >= 0 && diff_max.q >= 0 && diff_max.r >= 0) {
		Hex capacity_min = g->capacity_min;
		Hex diff_cap_min = hex_sub(h, capacity_min);
		i32 i = diff_cap_min.q + diff_cap_min.r * g->r_spacing;
		return g->data[i];
	} else {
		return NULL;
	}
}

u32 group_distance(Group *g, Hex h)
{
	Hex min = g->min;
	Hex max = g->max;
	Hex diff_min = hex_sub(min, h);
	Hex diff_max = hex_sub(h, max);

	if (diff_min.q > 0) {
		if (diff_min.r > diff_min.q) {
			return diff_min.r;
		} else if (diff_max.r > diff_min.q) {
			return diff_max.r;
		} else {
			return diff_min.q;
		}
	} else if (diff_max.q > 0) {
		if (diff_min.r > diff_max.q) {
			return diff_min.r;
		} else if (diff_max.r > diff_max.q) {
			return diff_max.r;
		} else {
			return diff_max.q;
		}
	} else if (diff_min.r > 0) {
		if (diff_min.q > diff_min.r) {
			return diff_min.q;
		} else if (diff_max.q > diff_min.r) {
			return diff_max.q;
		} else {
			return diff_min.r;
		}
	} else if (diff_max.r > 0) {
		if (diff_min.q > diff_max.r) {
			return diff_min.q;
		} else if (diff_max.q > diff_max.r) {
			return diff_max.q;
		} else {
			return diff_max.r;
		}
	} else {
		return 0;
	}
}

// Returns new_capacity - old_capacity
u32 group_set(Group *g, Hex h, void *datum)
{
	void **old_data = g->data;
	Hex min = g->min;
	Hex max = g->max;
	Hex capacity_min = g->capacity_min;
	Hex diff_min = hex_sub(h, min);
	Hex diff_max = hex_sub(max, h);
	Hex diff_cap_min = hex_sub(h, capacity_min);
	u32 r_spacing = g->r_spacing;
	i32 i = diff_cap_min.q + diff_cap_min.r * r_spacing;

	assert(datum != NULL);

	if (diff_min.q >= 0 && diff_min.r >= 0 && diff_max.q >= 0 && diff_max.r >= 0) {
		if (g->count == 0) {
			g->min = h;
			g->max = h;
		}

		if (old_data[i] == NULL) {
			++g->count;
		}
		old_data[i] = datum;
		return 0;
	}

	if (g->count == 0) {
		g->min = h;
		g->max = h;
	} else {
		if (diff_min.q < 0) {
			g->min.q += diff_min.q;
		} else if (diff_max.q < 0) {
			g->max.q -= diff_max.q;
		}

		if (diff_min.r < 0) {
			g->min.r += diff_min.r;
		} else if (diff_max.r < 0) {
			g->max.r -= diff_max.r;
		}
	}

	Hex capacity_max = g->capacity_max;
	Hex diff_cap_max = hex_sub(capacity_max, h);

	if (diff_cap_min.q >= 0 && diff_cap_min.r >= 0 && diff_cap_max.q >= 0 && diff_cap_max.r >= 0) {
		old_data[i] = datum;
		++g->count;
		return 0;
	}

	void **data;
	u32 capacity;
	i32 diff;
	u32 old_capacity = g->capacity;
	Hex old_capacity_min = capacity_min;
	u32 old_r_spacing = r_spacing;

	if (diff_cap_min.q < 0) {
		capacity_min.q += diff_cap_min.q;
		diff = capacity_max.q - capacity_min.q + 1;
		capacity_min.q -= (i32) round(diff * GROUP_CAPACITY_GROWTH_FACTOR);
	} else if (diff_cap_max.q < 0) {
		capacity_max.q -= diff_cap_max.q;
		diff = capacity_max.q - capacity_min.q + 1;
		capacity_max.q += (i32) round(diff * GROUP_CAPACITY_GROWTH_FACTOR);
	}

	if (diff_cap_min.r < 0) {
		capacity_min.r += diff_cap_min.r;
		diff = capacity_max.r - capacity_min.r + 1;
		capacity_min.r -= (i32) round(diff * GROUP_CAPACITY_GROWTH_FACTOR);
	} else if (diff_cap_max.r < 0) {
		capacity_max.r -= diff_cap_max.r;
		diff = capacity_max.r - capacity_min.r + 1;
		capacity_max.r += (i32) round(diff * GROUP_CAPACITY_GROWTH_FACTOR);
	}

	r_spacing = capacity_max.q - capacity_min.q + 1;
	capacity = (capacity_max.r - capacity_min.r + 1) * r_spacing;
	data = calloc(capacity, sizeof *data);

	for (i32 r = min.r; r <= max.r; ++r) {
		i32 r_diff = r - capacity_min.r;
		i32 i_offset = r_diff * r_spacing - capacity_min.q;
		i32 old_r_diff = min.r - old_capacity_min.r;
		i32 old_i_offset = old_r_diff * old_r_spacing - old_capacity_min.q;

		for (i32 q = min.q; q <= max.q; ++q) {
			data[q + i_offset] = old_data[q + old_i_offset];
		}
	}

	free(old_data);

	diff_cap_min = hex_sub(h, capacity_min);
	i = diff_cap_min.q + diff_cap_min.r * r_spacing;
	data[i] = datum;
	++g->count;
	g->data = data;
	g->r_spacing = r_spacing;
	g->capacity = capacity;
	g->capacity_min = capacity_min;
	g->capacity_max = capacity_max;

	return capacity - old_capacity;
}

// Returns 1 if item existed and was removed, 0 if not
u8 group_remove(Group *g, Hex h)
{
	void **data = g->data;
	Hex min = g->min;
	Hex max = g->max;
	Hex capacity_min = g->capacity_min;
	Hex diff_min = hex_sub(h, min);
	Hex diff_max = hex_sub(max, h);
	Hex diff_cap_min = hex_sub(h, capacity_min);
	u32 count = g->count;
	u32 r_spacing = g->r_spacing;
	i32 i = diff_cap_min.q + diff_cap_min.r * r_spacing;

	if (
		count == 0 ||
		diff_min.q < 0 || diff_min.r < 0 || diff_max.q < 0 || diff_max.r < 0 ||
		data[i] == NULL
	) {
		return 0;
	}

	data[i] = NULL;
	g->count = --count;

	if (count == 0) {
		g->min = g->capacity_min;
		g->max = g->capacity_max;
		return 1;
	}

	if (diff_min.q == 0 || diff_min.r == 0 || diff_max.q == 0 || diff_min.r == 0) {
		Hex new_min = EMPTY_GROUP_MIN;
		Hex new_max = EMPTY_GROUP_MAX;

		for (i32 r = min.r; r <= max.r; ++r) {
			i32 r_diff = r - capacity_min.r;
			i32 i_offset = r_diff * r_spacing - capacity_min.q;

			for (i32 q = min.q; q <= max.q; ++q) {
				if (data[q + i_offset] != NULL) {
					new_max.r = r;
					if (r < new_min.r) {
						new_min.r = r;
					}
					if (q > new_max.q) {
						new_max.q = q;
					}
					if (q < new_min.q) {
						new_min.q = q;
					}
				}
			}
		}

		g->min = new_min;
		g->max = new_max;
	}

	return 1;
}

void group_move(Group *g, Hex move_by)
{
	i32 q = move_by.q;
	i32 r = move_by.r;

	g->min.q += q;
	g->min.r += r;
	g->max.q += q;
	g->max.r += r;
	g->capacity_min.q += q;
	g->capacity_min.r += r;
	g->capacity_max.q += q;
	g->capacity_max.r += r;
}

u8 group_equal(Group *a, Group *b, GroupCompare compare_fn)
{
	u32 a_count = a->count;
	u32 b_count = b->count;
	Hex a_min = a->min;
	Hex b_min = b->min;
	Hex a_max_diff = hex_sub(a->max, a_min);
	Hex b_max_diff = hex_sub(b->max, b_min);

	if (a_count == 0 && b_count == 0) {
		return 1;
	}

	if (
		a->count != b->count ||
		!hex_equal(a_max_diff, b_max_diff)
	) {
		return 0;
	}

	Hex a_capacity_min = a->capacity_min;
	Hex b_capacity_min = b->capacity_min;
	u32 a_r_spacing = a->r_spacing;
	u32 b_r_spacing = b->r_spacing;
	void **a_data = a->data;
	void **b_data = b->data;

	for (i32 r = 0; r <= a_max_diff.r; ++r) {
		i32 a_r_diff = r + a_min.r - a_capacity_min.r;
		i32 b_r_diff = r + b_min.r - b_capacity_min.r;
		i32 a_i_offset = a_r_diff * a_r_spacing + a_min.q - a_capacity_min.q;
		i32 b_i_offset = b_r_diff * b_r_spacing + b_min.q - b_capacity_min.q;

		for (i32 q = 0; q <= a_max_diff.q; ++q) {
			void *a = a_data[q + a_i_offset];
			void *b = b_data[q + b_i_offset];

			if (a != NULL && b != NULL) {
				if (compare_fn(a, b) != 0) {
					return 0;
				}
			} else if (a != NULL || b != NULL) {
				return 0;
			}
		}
	}

	return 1;
}

void group_each(Group *g, void *context, GroupEach each_fn)
{
	void **data = g->data;
	Hex min = g->min;
	Hex max = g->max;
	Hex capacity_min = g->capacity_min;
	u32 r_spacing = g->r_spacing;

	for (i32 r = min.r; r <= max.r; ++r) {
		i32 r_diff = r - capacity_min.r;
		i32 i_offset = r_diff * r_spacing - capacity_min.q;

		for (i32 q = min.q; q <= max.q; ++q) {
			void *datum = data[q + i_offset];
			if (datum != NULL) {
				Hex h = {.q = q, .r = r};
				each_fn(context, h, datum);
			}
		}
	}
}
