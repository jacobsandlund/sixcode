#include <math.h>
#include "quad.h"
#include "space.h"

i8 quad_contains(Quad *q, Hex h)
{
	return (
		q->min.c <= h.c && h.c <= q->max.c &&
		q->min.r <= h.r && h.r <= q->max.r
	);
}

i8 quad_contains_quad(Quad *outer, Quad *inner)
{
	return (
		outer->min.c <= inner->min.c &&
		outer->min.r <= inner->min.r &&
		outer->max.c >= inner->max.c &&
		outer->max.r >= inner->max.r
	);
}

void quad_expand_quad(Quad *out_q, Quad *q, Hex h)
{
	out_q->min.c = h.c < q->min.c ? h.c : q->min.c;
	out_q->min.r = h.r < q->min.r ? h.r : q->min.r;
	out_q->max.c = h.c > q->max.c ? h.c : q->max.c;
	out_q->max.r = h.r > q->max.r ? h.r : q->max.r;
}

void quad_block_align(Quad *out_q, Quad *q, Hex block_size)
{
	out_q->min.c = q->min.c & ~(block_size.c - 1);
	out_q->min.r = q->min.r & ~(block_size.r - 1);
	out_q->max.c = q->max.c | (block_size.c - 1);
	out_q->max.r = q->max.r | (block_size.r - 1);
}

void quad_resize(Quad *out_q, Quad *q, Hex size_delta)
{
	out_q->min.c = q->min.c - size_delta.c;
	out_q->min.r = q->min.r - size_delta.r;
	out_q->max.c = q->max.c + size_delta.c;
	out_q->max.r = q->max.r + size_delta.r;
}

void quad_intersect(Quad *out_q, Quad *a, Quad *b)
{
	out_q->min.c = a->min.c > b->min.c ? a->min.c : b->min.c;
	out_q->min.r = a->min.r > b->min.r ? a->min.r : b->min.r;
	out_q->max.c = a->max.c < b->max.c ? a->max.c : b->max.c;
	out_q->max.r = a->max.r < b->max.r ? a->max.r : b->max.r;
}

void quad_space_hex_to_storage(Quad *out_q, Quad *q)
{
	out_q->min = space_hex_to_storage(q->min);
	out_q->max = space_hex_to_storage(q->max);
}

void quad_to_size_quad(SizeQuad *out_sq, Quad *q)
{
	out_sq->min = q->min;
	out_sq->size = (Hex) {
		q->max.c - q->min.c + 1,
		q->max.r - q->min.r + 1,
	};
}

void quad_to_storage_space_size_quad(SizeQuad *storage_quad, Quad *styles_quad)
{
	Quad storage_space_quad;
	quad_space_hex_to_storage(&storage_space_quad, styles_quad);
	quad_to_size_quad(storage_quad, &storage_space_quad);
}

void size_quad_even_align(SizeQuad *out_sq, SizeQuad *sq)
{
	i8 odd_row = sq->min.r & 1;
	out_sq->min.c = sq->min.c;
	out_sq->min.r = sq->min.r & ~1;
	out_sq->size.c = sq->size.c;
	out_sq->size.r = sq->size.r + odd_row;
}

i32 size_quad_capacity(SizeQuad *sq)
{
	return sq->size.c * sq->size.r;
}
