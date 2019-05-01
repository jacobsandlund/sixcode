#ifndef __QUAD_H__
#define __QUAD_H__

#include "sixcode.h"
#include "hex.h"

typedef struct {
	Hex min;
	Hex max;
} Quad;

typedef struct {
	Hex min;
	Hex size;
} SizeQuad;

i8 quad_contains(Quad *q, Hex h);
i8 quad_contains_quad(Quad *outer, Quad *inner);
void quad_from_hexes(Quad *q, Hex h1, Hex h2);
void quad_expand_for_hex(Quad *out_q, Quad *q, Hex h);
void quad_expand_for_quad(Quad *out_q, Quad *a, Quad *b);
void quad_block_align(Quad *out_q, Quad *q, Hex block_size);
void quad_resize_by_delta(Quad *out_q, Quad *q, Hex delta);
void quad_intersect(Quad *out_q, Quad *a, Quad *b);
void quad_hex_to_storage(Quad *out_q, Quad *q);

void quad_to_size_quad(SizeQuad *out_sq, Quad *q);
void quad_to_storage_size_quad(SizeQuad *out_sq, Quad *q);

void size_quad_even_align(SizeQuad *out_sq, SizeQuad *sq);
i32 size_quad_capacity(SizeQuad *sq);

#endif // __QUAD_H__
