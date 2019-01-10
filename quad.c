#include <math.h>
#include "quad.h"

#define QUAD_OUT_OF_BOUNDS 1073741824

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

Hex quad_size(Quad qd)
{
       Hex size = {
               .q = qd.max.q - qd.min.q + 1,
               .r = qd.max.r - qd.min.r + 1,
       };

       return size;
}

i8 quad_empty(Quad qd)
{
	return qd.max.q < qd.min.q || qd.max.r < qd.min.r;
}

Quad quad_expand(Quad qd, Hex include_hex)
{
	qd.min.q = include_hex.q < qd.min.q ? include_hex.q : qd.min.q;
	qd.min.r = include_hex.r < qd.min.r ? include_hex.r : qd.min.r;
	qd.max.q = include_hex.q > qd.max.q ? include_hex.q : qd.max.q;
	qd.max.r = include_hex.r > qd.max.r ? include_hex.r : qd.max.r;

	return qd;
}

Quad quad_capacity_quad(Quad qd, Hex extra_capacity, f64 extra_capacity_growth_factor)
{
	Hex original_size = quad_size(qd);
	extra_capacity.q += (i32) round(original_size.q * extra_capacity_growth_factor);
	extra_capacity.r += (i32) round(original_size.r * extra_capacity_growth_factor);

	Quad capacity_quad = {
		.min = hex_sub(qd.min, extra_capacity),
		.max = hex_add(qd.max, extra_capacity),
	};

	return capacity_quad;
}

Quad quad_move(Quad qd, Hex move_by)
{
	qd.min = hex_add(qd.min, move_by);
	qd.max = hex_add(qd.max, move_by);
	return qd;
}
