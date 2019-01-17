#ifndef __QUAD_H__
#define __QUAD_H__

#include "hex.h"
#include "sixcode.h"

typedef struct {
	Hex min;
	Hex max;
} Quad;

typedef struct {
	FloatHex min;
	FloatHex max;
} FloatQuad;

extern const Quad QUAD_EMPTY;

i8 quad_contains(Quad q, Hex h);
i8 quad_on_edge(Quad q, Hex h);
i32 quad_index(Quad q, Hex h);
i32 quad_capacity(Quad q);
Hex quad_size(Quad q);
i8 quad_empty(Quad q);
Quad quad_expand(Quad q, Hex include_hex);
Quad quad_capacity_quad(Quad q, Hex extra_capacity, f64 extra_capacity_growth_factor);
Quad quad_move(Quad q, Hex move_by);

#endif // __QUAD_H__
