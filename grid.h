#ifndef __GRID_H__
#define __GRID_H__

#include "bit-array.h"
#include "hex.h"
#include "quad.h"
#include "sixcode.h"

typedef struct {
	Quad quad;
	Quad capacity_quad;
	Hex extra_capacity;
	f64 extra_capacity_growth_factor;
	i32 capacity;
	i32 set_count;
	BitArray *set;
	void **data;
	i8 with_data;
} Grid;

#define GRID_WITH_DATA 1
#define GRID_NO_DATA 0

typedef void (*GridEach)(void *context, Hex h, void *data);

Grid *grid_create(Hex extra_capacity, f64 extra_capacity_growth_factor, i8 with_data);
void grid_destroy(Grid *g);
void grid_clear(Grid *g);
//void grid_ensure_capacity(Grid *g, Quad capacity_quad);
void *grid_get(Grid *g, Hex h);
i8 grid_has(Grid *g, Hex h);
void grid_add(Grid *g, Hex h, void *data);

// Returns 1 if item existed and was removed, 0 if not
i8 grid_remove(Grid *g, Hex h);

void grid_move(Grid *g, Hex move_by);
void grid_each(Grid *g, Quad quad, void *context, GridEach each_fn);

#endif // __GRID_H__
