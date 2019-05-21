#ifndef WORLD_H
#define WORLD_H

#include "spacetime.h"
#include "grid.h"
#include "viewport.h"

typedef struct {
	Grid grid;
	Viewport viewport;
} World;

void world_initialize(World *w);
void world_loop_tick(World *w, Renderer *r);
void world_toggle_hex_at_point(World *w, vec2 v);

#endif // WORLD_H
