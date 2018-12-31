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

Group *group_init(Hex capacity_min, Hex capacity_max)
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

void *group_get(Group *g, Hex h)
{
	Hex min = g->min;
	Hex max = g->max;
	i32 q_diff_min = h.q - min.q;
	i32 q_diff_max = max.q - h.q;
	i32 r_diff_min = h.r - min.r;
	i32 r_diff_max = max.r - h.r;

	if (q_diff_min >= 0 && q_diff_max >= 0 && r_diff_min >= 0 && r_diff_max >= 0) {
		Hex capacity_min = g->capacity_min;
		i32 q_diff_cap_min = h.q - capacity_min.q;
		i32 r_diff_cap_min = h.r - capacity_min.r;
		i32 i = q_diff_cap_min + r_diff_cap_min * g->r_spacing;
		return g->data[i];
	} else {
		return NULL;
	}
}

u32 group_distance(Group *g, Hex h)
{
	Hex min = g->min;
	Hex max = g->max;
	i32 q_diff_min = min.q - h.q;
	i32 q_diff_max = h.q - max.q;
	i32 r_diff_min = min.r - h.r;
	i32 r_diff_max = h.r - max.r;

	if (q_diff_min > 0) {
		if (r_diff_min > q_diff_min) {
			return r_diff_min;
		} else if (r_diff_max > q_diff_min) {
			return r_diff_max;
		} else {
			return q_diff_min;
		}
	} else if (q_diff_max > 0) {
		if (r_diff_min > q_diff_max) {
			return r_diff_min;
		} else if (r_diff_max > q_diff_max) {
			return r_diff_max;
		} else {
			return q_diff_max;
		}
	} else if (r_diff_min > 0) {
		if (q_diff_min > r_diff_min) {
			return q_diff_min;
		} else if (q_diff_max > r_diff_min) {
			return q_diff_max;
		} else {
			return r_diff_min;
		}
	} else if (r_diff_max > 0) {
		if (q_diff_min > r_diff_max) {
			return q_diff_min;
		} else if (q_diff_max > r_diff_max) {
			return q_diff_max;
		} else {
			return r_diff_max;
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
	u32 r_spacing = g->r_spacing;
	i32 q_diff_min = h.q - min.q;
	i32 q_diff_max = max.q - h.q;
	i32 r_diff_min = h.r - min.r;
	i32 r_diff_max = max.r - h.r;
	i32 q_diff_cap_min = h.q - capacity_min.q;
	i32 r_diff_cap_min = h.r - capacity_min.r;
	i32 i = q_diff_cap_min + r_diff_cap_min * r_spacing;

	assert(datum != NULL);

	if (q_diff_min >= 0 && q_diff_max >= 0 && r_diff_min >= 0 && r_diff_max >= 0) {
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
		if (q_diff_min < 0) {
			g->min.q += q_diff_min;
		} else if (q_diff_max < 0) {
			g->max.q -= q_diff_max;
		}

		if (r_diff_min < 0) {
			g->min.r += r_diff_min;
		} else if (r_diff_max < 0) {
			g->max.r -= r_diff_max;
		}
	}

	Hex capacity_max = g->capacity_max;
	i32 q_diff_cap_max = capacity_max.q - h.q;
	i32 r_diff_cap_max = capacity_max.r - h.r;

	if (q_diff_cap_min >= 0 && q_diff_cap_max >= 0 && r_diff_cap_min >= 0 && r_diff_cap_max >= 0) {
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

	if (q_diff_cap_min < 0) {
		capacity_min.q += q_diff_cap_min;
		diff = capacity_max.q - capacity_min.q + 1;
		capacity_min.q -= (i32) round(diff * GROUP_CAPACITY_GROWTH_FACTOR);
	} else if (q_diff_cap_max < 0) {
		capacity_max.q -= q_diff_cap_max;
		diff = capacity_max.q - capacity_min.q + 1;
		capacity_max.q += (i32) round(diff * GROUP_CAPACITY_GROWTH_FACTOR);
	}

	if (r_diff_cap_min < 0) {
		capacity_min.r += r_diff_cap_min;
		diff = capacity_max.r - capacity_min.r + 1;
		capacity_min.r -= (i32) round(diff * GROUP_CAPACITY_GROWTH_FACTOR);
	} else if (r_diff_cap_max < 0) {
		capacity_max.r -= r_diff_cap_max;
		diff = capacity_max.r - capacity_min.r + 1;
		capacity_max.r += (i32) round(diff * GROUP_CAPACITY_GROWTH_FACTOR);
	}

	r_spacing = capacity_max.q - capacity_min.q + 1;
	capacity = (capacity_max.r - capacity_min.r + 1) * r_spacing;
	data = malloc(capacity * sizeof *data);

	for (i32 r = min.r; r <= max.r; ++r) {
		i32 r_diff = r - capacity_min.r;
		i32 i_offset = r_diff * r_spacing - capacity_min.q;
		i32 old_r_diff = min.r - old_capacity_min.r;
		i32 old_i_offset = old_r_diff * old_r_spacing - old_capacity_min.q;
		for (i32 q = min.q; q <= max.q; ++q) {
			data[q + i_offset] = old_data[q + old_i_offset];
		}
	}

	q_diff_cap_min = h.q - capacity_min.q;
	r_diff_cap_min = h.r - capacity_min.r;
	i = q_diff_cap_min + r_diff_cap_min * r_spacing;
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
	Hex new_min = EMPTY_GROUP_MIN;
	Hex new_max = EMPTY_GROUP_MAX;
	void **data = g->data;
	Hex min = g->min;
	Hex max = g->max;
	Hex capacity_min = g->capacity_min;
	u32 count = g->count;
	u32 r_spacing = g->r_spacing;
	i32 q_diff_min = h.q - min.q;
	i32 q_diff_max = max.q - h.q;
	i32 r_diff_min = h.r - min.r;
	i32 r_diff_max = max.r - h.r;
	i32 q_diff_cap_min = h.q - capacity_min.q;
	i32 r_diff_cap_min = h.r - capacity_min.r;
	i32 h_i = q_diff_cap_min + r_diff_cap_min * r_spacing;

	if (
		count == 0 ||
		q_diff_min < 0 || q_diff_max < 0 || r_diff_min < 0 || r_diff_max < 0 ||
		data[h_i] == NULL
	) {
		return 0;
	}

	data[h_i] = NULL;
	g->count = --count;

	if (count == 0) {
		g->min = g->capacity_min;
		g->max = g->capacity_max;
		return 1;
	}

	if (q_diff_min == 0 || q_diff_max == 0 || r_diff_min == 0 || r_diff_max == 0) {
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

u8 group_equal(Group *a, Group *b, GroupCompare compare_fn)
{
	Hex a_min = a->min;
	Hex b_min = b->min;
	Hex a_max = a->max;
	Hex b_max = b->max;

	if (
		a->count != b->count ||
		a_min.q != b_min.q ||
		a_min.r != b_min.r ||
		a_max.q != b_max.q ||
		a_max.r != b_max.r
	) {
		return 0;
	}

	Hex a_capacity_min = a->capacity_min;
	Hex b_capacity_min = b->capacity_min;
	u32 a_r_spacing = a->r_spacing;
	u32 b_r_spacing = b->r_spacing;
	void **a_data = a->data;
	void **b_data = b->data;

	for (i32 r = a_min.r; r <= a_max.r; ++r) {
		i32 a_r_diff = r - a_capacity_min.r;
		i32 b_r_diff = r - b_capacity_min.r;
		i32 a_i_offset = a_r_diff * a_r_spacing - a_capacity_min.q;
		i32 b_i_offset = b_r_diff * b_r_spacing - b_capacity_min.q;
		for (i32 q = a_min.q; q <= a_max.q; ++q) {
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
