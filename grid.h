#ifndef __GRID_H__
#define __GRID_H__

#include "bit-array.h"
#include "hex.h"
#include "quad.h"
#include "sixcode.h"

typedef struct {
	Quad quad;
	i32 quad_size_c;
	BitArray *set;
	u8 *styles;
} Grid;

typedef void (*GridEach)(void *context, Hex h, u8 style);

void grid_initialize(Grid *g, Quad quad);
void grid_terminate(Grid *g);
void grid_ensure_capacity(Grid *g, Quad capacity_quad);
i32 grid_index(Grid *g, Hex h);
u8 grid_get(Grid *g, Hex h);
i8 grid_has(Grid *g, Hex h);
void grid_set(Grid *g, Hex h, u8 style);
void grid_clear(Grid *g, Hex h);

void grid_each(Grid *g, Quad quad, void *context, GridEach each_fn);

#endif // __GRID_H__
