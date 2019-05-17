#ifndef GRID_H
#define GRID_H

#include "sixcode.h"
#include "quad.h"

typedef struct {
	Quad quad;
	Quad styles_quad;  // quad with extra border
	SizeQuad size_quad;
	u8 *styles;
} Grid;

void grid_initialize(Grid *g);
void grid_terminate(Grid *g);
u8 grid_get(Grid *g, ivec2 h);
void grid_set(Grid *g, ivec2 h, u8 style);
void grid_clear(Grid *g, ivec2 h);

#endif // GRID_H
