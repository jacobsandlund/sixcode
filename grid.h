#ifndef __GRID_H__
#define __GRID_H__

#include "sixcode.h"
#include "hex.h"
#include "quad.h"

typedef struct {
	Quad quad;
	Quad capacity_quad;
	u32 data_count;
	void **data;
} Grid;


// Return:
//   -1: a < b
//    0: a == b
//    1: a > b
typedef i8 (*GridCompare)(void *a, void *b);

typedef void (*GridEach)(void *context, Hex h, void *datum);

Grid *grid_create(Quad capacity_quad);
void grid_destroy(Grid *g);
void *grid_get(Grid *g, Hex h);
void grid_set(Grid *g, Hex h, void *datum);

// Returns 1 if item existed and was removed, 0 if not
u8 grid_remove(Grid *g, Hex h);

void grid_move(Grid *g, Hex move_by);
void grid_each(Grid *g, void *context, GridEach each_fn);

#endif // __GRID_H__
