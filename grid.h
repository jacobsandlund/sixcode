#ifndef __GRID_H__
#define __GRID_H__

#include "sixcode.h"
#include "hex.h"
#include "quad.h"

typedef struct {
	Quad quad;
	Quad styles_quad;  // quad with extra border
	SizeQuad storage_quad;
	u8 *styles;
} Grid;

void grid_quad_to_styles_quad(Quad *styles_quad, Quad *quad);
void grid_initialize(Grid *g, Quad *quad);
void grid_terminate(Grid *g);
u8 grid_get(Grid *g, Hex h);
void grid_set(Grid *g, Hex h, u8 style);
void grid_clear(Grid *g, Hex h);
void grid_expand_quad(Grid *g, Quad *quad);

#endif // __GRID_H__
