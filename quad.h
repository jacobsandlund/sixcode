#ifndef __QUAD_H__
#define __QUAD_H__

#include "sixcode.h"
#include "hex.h"

typedef struct {
	Hex min;
	Hex max;
} Quad;

extern const Quad QUAD_ZERO;
extern const Quad QUAD_EMPTY;

u8 quad_contains(Quad qd, Hex h);
u8 quad_on_edge(Quad qd, Hex h);
u8 quad_empty(Quad qd);
u32 quad_distance(Quad qd, Hex h);
i32 quad_index(Quad qd, Hex h);
i32 quad_capacity(Quad qd);
Quad quad_expand(Quad qd, Hex include_hex, f64 growth_factor);
Quad quad_move(Quad qd, Hex move_by);
u8 quad_equal(Quad qd, Quad b);

#endif // __QUAD_H__
