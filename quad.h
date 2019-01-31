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
i8 quad_is_simple(Quad *q);

void storage_quad_from_quad(StorageQuad *sq, Quad *q);
i32 storage_quad_capacity(StorageQuad *sq);

#endif // __QUAD_H__
