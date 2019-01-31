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
		inner->max.c <= outer->max.c &&
		inner->max.r <= outer->max.r
	);
}

i8 quad_is_simple(Quad *q)
{
	return (
		!(q->min.c & 1) &&
		!(q->min.r & 1) &&
		(q->max.r & 1) &&
		(q->max.c & 1) &&
		(q->max.c & 2)
	);
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
