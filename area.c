#include <math.h>
#include "area.h"

#define AREA_OUT_OF_BOUNDS 1073741824

const Area AREA_ZERO = {
	.min = {.q = 0, .r = 0},
	.max = {.q = 0, .r = 0},
};

const Area AREA_EMPTY = {
	.min = {.q = AREA_OUT_OF_BOUNDS, .r = AREA_OUT_OF_BOUNDS},
	.max = {.q = -AREA_OUT_OF_BOUNDS, .r = -AREA_OUT_OF_BOUNDS},
};

u8 area_contains(Area a, Hex h)
{
	return  a.min.q <= h.q && h.q <= a.max.q &&
		a.min.r <= h.r && h.r <= a.max.r;
}

u8 area_on_edge(Area a, Hex h)
{
	return h.q == a.min.q || h.q == a.max.q || h.r == a.min.r || h.r == a.max.r;
}

u8 area_empty(Area a)
{
	return a.min.q > a.max.q || a.min.r > a.max.r;
}

u32 area_distance(Area a, Hex h)
{
	Hex diff_min = hex_sub(a.min, h);
	Hex diff_max = hex_sub(h, a.max);

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

i32 area_index(Area a, Hex h)
{
	Hex diff_min = hex_sub(h, a.min);
	u32 r_spacing = a.max.q - a.min.q + 1;
	return diff_min.q + diff_min.r * r_spacing;
}

i32 area_capacity(Area a)
{
	return area_index(a, a.max) + 1;
}

Area area_expand(Area a, Hex include_hex, f64 growth_factor)
{
	Hex diff_min = hex_sub(a.min, include_hex);
	Hex diff_max = hex_sub(include_hex, a.max);

	if (diff_min.q > 0) {
		a.min.q -= diff_min.q;
		a.min.q -= (i32) round((a.max.q - a.min.q) * growth_factor);
	} else if (diff_max.q > 0) {
		a.max.q += diff_max.q;
		a.max.q += (i32) round((a.max.q - a.min.q) * growth_factor);
	}

	if (diff_min.r > 0) {
		a.min.r -= diff_min.r;
		a.min.r -= (i32) round((a.max.r - a.min.r) * growth_factor);
	} else if (diff_max.r > 0) {
		a.max.r += diff_max.r;
		a.max.r += (i32) round((a.max.r - a.min.r) * growth_factor);
	}

	return a;
}

Area area_move(Area a, Hex move_by)
{
	a.min = hex_add(a.min, move_by);
	a.max = hex_add(a.max, move_by);
	return a;
}

u8 area_equal(Area a, Area b)
{
	return hex_equal(a.min, b.min) && hex_equal(a.max, b.max);
}
