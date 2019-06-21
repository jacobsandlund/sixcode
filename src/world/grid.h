#ifndef _WorldGrid_h
#define _WorldGrid_h

#include "spacetime.h"
#include "math/quad.h"

typedef struct {
    Quad quad;
    SizeQuad size_quad;
    u8 *styles;
} WorldGrid;

void WorldGridInit(WorldGrid *g, i32 size);
void WorldGridDestroy(WorldGrid *g);
u8 WorldGridGet(WorldGrid *g, int2 h);
void WorldGridSet(WorldGrid *g, int2 h, u8 style);
void WorldGridClear(WorldGrid *g, int2 h);

#endif // _WorldGrid_h
