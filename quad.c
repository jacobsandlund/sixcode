#include "quad.h"
#include <math.h>
#include "hex-coords.h"

bool quad_contains(Quad *q, ivec2 h)
{
	return (
		q->min.x <= h.x && h.x <= q->max.x &&
		q->min.y <= h.y && h.y <= q->max.y
	);
}

bool quad_contains_quad(Quad *outer, Quad *inner)
{
	return (
		outer->min.x <= inner->min.x &&
		outer->min.y <= inner->min.y &&
		outer->max.x >= inner->max.x &&
		outer->max.y >= inner->max.y
	);
}

void quad_from_hexes(Quad *q, ivec2 h1, ivec2 h2)
{
	q->min.x = h1.x < h2.x ? h1.x : h2.x;
	q->min.y = h1.y < h2.y ? h1.y : h2.y;
	q->max.x = h1.x > h2.x ? h1.x : h2.x;
	q->max.y = h1.y > h2.y ? h1.y : h2.y;
}

void quad_expand_for_hex(Quad *out_q, Quad *q, ivec2 h)
{
	out_q->min.x = h.x < q->min.x ? h.x : q->min.x;
	out_q->min.y = h.y < q->min.y ? h.y : q->min.y;
	out_q->max.x = h.x > q->max.x ? h.x : q->max.x;
	out_q->max.y = h.y > q->max.y ? h.y : q->max.y;
}

void quad_expand_for_quad(Quad *out_q, Quad *a, Quad *b)
{
	out_q->min.x = a->min.x < b->min.x ? a->min.x : b->min.x;
	out_q->min.y = a->min.y < b->min.y ? a->min.y : b->min.y;
	out_q->max.x = a->max.x > b->max.x ? a->max.x : b->max.x;
	out_q->max.y = a->max.y > b->max.y ? a->max.y : b->max.y;
}

void quad_block_align(Quad *out_q, Quad *q, ivec2 block_size)
{
	out_q->min.x = q->min.x & ~(block_size.x - 1);
	out_q->min.y = q->min.y & ~(block_size.y - 1);
	out_q->max.x = q->max.x | (block_size.x - 1);
	out_q->max.y = q->max.y | (block_size.y - 1);
}

void quad_resize_by_delta(Quad *out_q, Quad *q, ivec2 delta)
{
	out_q->min.x = q->min.x - delta.x;
	out_q->min.y = q->min.y - delta.y;
	out_q->max.x = q->max.x + delta.x;
	out_q->max.y = q->max.y + delta.y;
}

void quad_intersect(Quad *out_q, Quad *a, Quad *b)
{
	out_q->min.x = a->min.x > b->min.x ? a->min.x : b->min.x;
	out_q->min.y = a->min.y > b->min.y ? a->min.y : b->min.y;
	out_q->max.x = a->max.x < b->max.x ? a->max.x : b->max.x;
	out_q->max.y = a->max.y < b->max.y ? a->max.y : b->max.y;
}

void quad_hex_coords_to_storage(Quad *out_q, Quad *q)
{
	out_q->min = hex_coords_to_storage(q->min);
	out_q->max = hex_coords_to_storage(q->max);
}

void quad_to_size_quad(SizeQuad *out_sq, Quad *q)
{
	out_sq->min = q->min;
	out_sq->size = (ivec2) {
		q->max.x - q->min.x + 1,
		q->max.y - q->min.y + 1,
	};
}

void quad_to_storage_size_quad(SizeQuad *storage_quad, Quad *styles_quad)
{
	Quad q;
	quad_hex_coords_to_storage(&q, styles_quad);
	quad_to_size_quad(storage_quad, &q);
}

void size_quad_even_align(SizeQuad *out_sq, SizeQuad *sq)
{
	int odd_row = sq->min.y & 1;
	out_sq->min.x = sq->min.x;
	out_sq->min.y = sq->min.y & ~1;
	out_sq->size.x = sq->size.x;
	out_sq->size.y = sq->size.y + odd_row;
}

int size_quad_capacity(SizeQuad *sq)
{
	return sq->size.x * sq->size.y;
}
