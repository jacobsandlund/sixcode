#include "math/quad.h"
#include <math.h>

bool quad_contains(Quad *q, int2 h)
{
    return (
        q->min.x <= h.x && h.x <= q->max.x &&
        q->min.y <= h.y && h.y <= q->max.y
    );
}

void quad_from_hexes(Quad *q, int2 h1, int2 h2)
{
    q->min.x = h1.x < h2.x ? h1.x : h2.x;
    q->min.y = h1.y < h2.y ? h1.y : h2.y;
    q->max.x = h1.x > h2.x ? h1.x : h2.x;
    q->max.y = h1.y > h2.y ? h1.y : h2.y;
}

void quad_intersect(Quad *out_q, Quad *a, Quad *b)
{
    out_q->min.x = a->min.x > b->min.x ? a->min.x : b->min.x;
    out_q->min.y = a->min.y > b->min.y ? a->min.y : b->min.y;
    out_q->max.x = a->max.x < b->max.x ? a->max.x : b->max.x;
    out_q->max.y = a->max.y < b->max.y ? a->max.y : b->max.y;
}

void quad_to_size_quad(SizeQuad *out_sq, Quad *q)
{
    out_sq->min = q->min;
    out_sq->size = (int2) {
        q->max.x - q->min.x + 1,
        q->max.y - q->min.y + 1,
    };
}

void size_quad_even_align(SizeQuad *out_sq, SizeQuad *sq)
{
    i64 odd_row = sq->min.y & 1;
    out_sq->min.x = sq->min.x;
    out_sq->min.y = sq->min.y & ~1;
    out_sq->size.x = sq->size.x;
    out_sq->size.y = (i32) (sq->size.y + odd_row);
}

i64 size_quad_capacity(SizeQuad *sq)
{
    return sq->size.x * sq->size.y;
}
