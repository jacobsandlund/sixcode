#ifndef __QUAD_H__
#define __QUAD_H__

#include "hex.h"
#include "sixcode.h"

typedef struct {
	Hex min;
	Hex max;
} Quad;

i8 quad_contains(Quad q, Hex h);
i8 quad_contains_quad(Quad outer, Quad inner);
Hex quad_size(Quad q);
i32 quad_capacity(Quad q);

#endif // __QUAD_H__
