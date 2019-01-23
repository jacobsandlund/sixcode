#include <math.h>
#include "quad.h"

i8 quad_contains(Quad q, Hex h)
{
	return  q.min.c <= h.c && h.c <= q.max.c &&
		q.min.r <= h.r && h.r <= q.max.r;
}

i8 quad_contains_quad(Quad outer, Quad inner)
{
	return  outer.min.c <= inner.min.c && inner.max.c <= outer.max.c &&
	        outer.min.r <= inner.min.r && inner.max.r <= outer.max.r;
}

Hex quad_size(Quad q)
{
       Hex size = {
               .c = q.max.c - q.min.c + 1,
               .r = q.max.r - q.min.r + 1,
       };

       return size;
}

i32 quad_capacity(Quad q)
{
	Hex size = quad_size(q);
	return size.c * size.r;
}
