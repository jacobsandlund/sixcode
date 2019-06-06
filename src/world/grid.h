#ifndef WorldGrid_h
#define WorldGrid_h

#include "spacetime.h"
#include "math/quad.h"

typedef struct {
    Quad quad;
    Quad styles_quad;  // quad with extra border
    SizeQuad size_quad;
    u8 *styles;
} WorldGrid;

void world_grid_init(WorldGrid *g);
void world_grid_destroy(WorldGrid *g);
u8 world_grid_get(WorldGrid *g, int2 h);
void world_grid_set(WorldGrid *g, int2 h, u8 style);
void world_grid_clear(WorldGrid *g, int2 h);

#endif // WorldGrid_h
