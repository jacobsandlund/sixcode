#include <math.h>
#include "quad.h"

#define QUAD_OUT_OF_BOUNDS 1073741824

const Quad QUAD_ZERO = {
	.min = {.q = 0, .r = 0},
	.max = {.q = 0, .r = 0},
};

const Quad QUAD_EMPTY = {
	.min = {.q = QUAD_OUT_OF_BOUNDS, .r = QUAD_OUT_OF_BOUNDS},
	.max = {.q = -QUAD_OUT_OF_BOUNDS, .r = -QUAD_OUT_OF_BOUNDS},
};

u8 quad_contains(Quad qd, Hex h)
{
	return  qd.min.q <= h.q && h.q <= qd.max.q &&
		qd.min.r <= h.r && h.r <= qd.max.r;
}

u8 quad_on_edge(Quad qd, Hex h)
{
	return h.q == qd.min.q || h.q == qd.max.q || h.r == qd.min.r || h.r == qd.max.r;
}

u8 quad_empty(Quad qd)
{
	return qd.min.q > qd.max.q || qd.min.r > qd.max.r;
}

u32 quad_distance(Quad qd, Hex h)
{
	Hex diff_min = hex_sub(qd.min, h);
	Hex diff_max = hex_sub(h, qd.max);

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

i32 quad_index(Quad qd, Hex h)
{
	Hex diff_min = hex_sub(h, qd.min);
	u32 r_spacing = qd.max.q - qd.min.q + 1;
	return diff_min.q + diff_min.r * r_spacing;
}

i32 quad_capacity(Quad qd)
{
	return quad_index(qd, qd.max) + 1;
}

Quad quad_expand(Quad qd, Hex include_hex, f64 growth_factor)
{
	Hex diff_min = hex_sub(qd.min, include_hex);
	Hex diff_max = hex_sub(include_hex, qd.max);

	if (diff_min.q > 0) {
		qd.min.q -= diff_min.q;
		qd.min.q -= (i32) round((qd.max.q - qd.min.q) * growth_factor);
	} else if (diff_max.q > 0) {
		qd.max.q += diff_max.q;
		qd.max.q += (i32) round((qd.max.q - qd.min.q) * growth_factor);
	}

	if (diff_min.r > 0) {
		qd.min.r -= diff_min.r;
		qd.min.r -= (i32) round((qd.max.r - qd.min.r) * growth_factor);
	} else if (diff_max.r > 0) {
		qd.max.r += diff_max.r;
		qd.max.r += (i32) round((qd.max.r - qd.min.r) * growth_factor);
	}

	return qd;
}

Quad quad_move(Quad qd, Hex move_by)
{
	qd.min = hex_add(qd.min, move_by);
	qd.max = hex_add(qd.max, move_by);
	return qd;
}

u8 quad_equal(Quad qd, Quad b)
{
	return hex_equal(qd.min, b.min) && hex_equal(qd.max, b.max);
}
