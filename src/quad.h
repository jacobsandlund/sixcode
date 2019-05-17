#ifndef QUAD_H
#define QUAD_H

#include "spacetime.h"

typedef struct {
	ivec2 min;
	ivec2 max;
} Quad;

typedef struct {
	ivec2 min;
	ivec2 size;
} SizeQuad;

bool quad_contains(Quad *q, ivec2 h);
void quad_from_hexes(Quad *q, ivec2 h1, ivec2 h2);
void quad_intersect(Quad *out_q, Quad *a, Quad *b);
void quad_to_size_quad(SizeQuad *out_sq, Quad *q);
void size_quad_even_align(SizeQuad *out_sq, SizeQuad *sq);
int size_quad_capacity(SizeQuad *sq);

#endif // QUAD_H
