#include <math.h>
#include "quad.h"

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

void quad_resize(Quad *out_q, Quad *q, i32 size_delta)
{
	out_q->min.c = q->min.c - 2 * size_delta;
	out_q->min.r = q->min.r - 1 * size_delta;
	out_q->max.c = q->max.c + 2 * size_delta;
	out_q->max.r = q->max.r + 1 * size_delta;
}

void quad_intersect(Quad *out_q, Quad *a, Quad *b)
{
	out_q->min.c = a->min.c > b->min.c ? a->min.c : b->min.c;
	out_q->min.r = a->min.r > b->min.r ? a->min.r : b->min.r;
	out_q->max.c = a->max.c < b->max.c ? a->max.c : b->max.c;
	out_q->max.r = a->max.r < b->max.r ? a->max.r : b->max.r;
}

void storage_quad_from_quad(StorageQuad *sq, Quad *q)
{
	i32 sq_max_c = q->max.c >> 1;
	sq->min = (Hex) {q->min.c >> 1, q->min.r};
	sq->size = (Hex) {
		sq_max_c - sq->min.c + 1,
		q->max.r - q->min.r + 1,
	};
}

i32 storage_quad_capacity(StorageQuad *sq)
{
	return sq->size.c * sq->size.r;
}
