#ifndef __QUAD_H__
#define __QUAD_H__

#include "hex.h"
#include "sixcode.h"

typedef struct {
	Hex min;
	Hex max;
} Quad;

typedef struct {
	Hex min;
	Hex size;
} StorageQuad;

i8 quad_contains(Quad *q, Hex h);
i8 quad_contains_quad(Quad *outer, Quad *inner);
void quad_expand_quad(Quad *out_q, Quad *q, Hex h);
void quad_block_align(Quad *out_q, Quad *q, Hex block_size);
void quad_resize(Quad *out_q, Quad *q, i32 size_delta);
void quad_intersect(Quad *out_q, Quad *a, Quad *b);

void storage_quad_from_quad(StorageQuad *sq, Quad *q);
i32 storage_quad_capacity(StorageQuad *sq);

#endif // __QUAD_H__
