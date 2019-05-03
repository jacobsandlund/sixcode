#ifndef __QUAD_H__
#define __QUAD_H__

#include "sixcode.h"

typedef struct {
	ivec2 min;
	ivec2 max;
} Quad;

typedef struct {
	ivec2 min;
	ivec2 size;
} SizeQuad;

bool quad_contains(Quad *q, ivec2 h);
bool quad_contains_quad(Quad *outer, Quad *inner);
void quad_from_hexes(Quad *q, ivec2 h1, ivec2 h2);
void quad_expand_for_hex(Quad *out_q, Quad *q, ivec2 h);
void quad_expand_for_quad(Quad *out_q, Quad *a, Quad *b);
void quad_block_align(Quad *out_q, Quad *q, ivec2 block_size);
void quad_resize_by_delta(Quad *out_q, Quad *q, ivec2 delta);
void quad_intersect(Quad *out_q, Quad *a, Quad *b);
void quad_hex_coords_to_storage(Quad *out_q, Quad *q);

void quad_to_size_quad(SizeQuad *out_sq, Quad *q);
void quad_to_storage_size_quad(SizeQuad *out_sq, Quad *q);

void size_quad_even_align(SizeQuad *out_sq, SizeQuad *sq);
int size_quad_capacity(SizeQuad *sq);

#endif // __QUAD_H__
