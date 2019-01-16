#include <math.h>
#include "quad.h"

#define QUAD_OUT_OF_BOUNDS 1073741824

const Quad QUAD_EMPTY = {
	.min = {.c = QUAD_OUT_OF_BOUNDS, .r = QUAD_OUT_OF_BOUNDS},
	.max = {.c = -QUAD_OUT_OF_BOUNDS, .r = -QUAD_OUT_OF_BOUNDS},
};

i8 quad_contains(Quad q, Hex h)
{
	return  q.min.c <= h.c && h.c <= q.max.c &&
		q.min.r <= h.r && h.r <= q.max.r;
}

i8 quad_on_edge(Quad q, Hex h)
{
	return h.c == q.min.c || h.c == q.max.c || h.r == q.min.r || h.r == q.max.r;
}

i32 quad_index(Quad q, Hex h)
{
	// Hex diff_min = hex_sub(h, q.min);
	i32 diff_min_c = h.c - q.min.c;
	i32 diff_min_r = h.r - q.min.r;
	i32 r_spacing = (q.max.c - q.min.c) / 2 + 1;
	return diff_min_c / 2 + diff_min_r * r_spacing;
}

i32 quad_capacity(Quad q)
{
	return quad_index(q, q.max) + 1;
}

Hex quad_size(Quad q)
{
       Hex size = {
               .c = q.max.c - q.min.c + 1,
               .r = q.max.r - q.min.r + 1,
       };

       return size;
}

i8 quad_empty(Quad q)
{
	return q.max.c < q.min.c || q.max.r < q.min.r;
}

Quad quad_expand(Quad q, Hex include_hex)
{
	q.min.c = include_hex.c < q.min.c ? include_hex.c : q.min.c;
	q.min.r = include_hex.r < q.min.r ? include_hex.r : q.min.r;
	q.max.c = include_hex.c > q.max.c ? include_hex.c : q.max.c;
	q.max.r = include_hex.r > q.max.r ? include_hex.r : q.max.r;

	return q;
}

Quad quad_capacity_quad(Quad q, Hex extra_capacity, f64 extra_capacity_growth_factor)
{
	Hex original_size = quad_size(q);
	extra_capacity.c += (i32) round(original_size.c * extra_capacity_growth_factor);
	extra_capacity.r += (i32) round(original_size.r * extra_capacity_growth_factor);

	Quad capacity_quad = {
		.min = hex_sub(q.min, extra_capacity),
		.max = hex_add(q.max, extra_capacity),
	};

	return capacity_quad;
}

Quad quad_move(Quad q, Hex move_by)
{
	q.min = hex_add(q.min, move_by);
	q.max = hex_add(q.max, move_by);
	return q;
}
