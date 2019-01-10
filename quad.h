#ifndef __QUAD_H__
#define __QUAD_H__

#include "sixcode.h"
#include "hex.h"

typedef struct {
	Hex min;
	Hex max;
} Quad;

extern const Quad QUAD_EMPTY;

u8 quad_contains(Quad qd, Hex h);
u8 quad_on_edge(Quad qd, Hex h);
i32 quad_index(Quad qd, Hex h);
i32 quad_capacity(Quad qd);
Hex quad_size(Quad qd);
i8 quad_empty(Quad qd);
Quad quad_expand(Quad qd, Hex include_hex);
Quad quad_capacity_quad(Quad qd, Hex extra_capacity, f64 extra_capacity_growth_factor);
Quad quad_move(Quad qd, Hex move_by);

#endif // __QUAD_H__
