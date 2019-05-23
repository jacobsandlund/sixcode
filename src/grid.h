#ifndef Grid_h
#define Grid_h

#include "spacetime.h"
#include "quad.h"

typedef struct {
	Quad quad;
	Quad styles_quad;  // quad with extra border
	SizeQuad size_quad;
	u8 *styles;
} Grid;

void grid_initialize(Grid *g);
void grid_terminate(Grid *g);
u8 grid_get(Grid *g, int2 h);
void grid_set(Grid *g, int2 h, u8 style);
void grid_clear(Grid *g, int2 h);

#endif // Grid_h
