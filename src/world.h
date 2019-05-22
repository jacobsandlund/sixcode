#ifndef WORLD_H
#define WORLD_H

#include "spacetime.h"
#include "grid.h"   
#include "viewport.h"
#include "view.h"

typedef struct {
	Grid grid;
	Viewport viewport;
} World;

void world_initialize(World *w);
void world_terminate(World *w);
void world_load(World *w);
void world_update(World *w, View *vw);
//void world_toggle_hex_at_point(World *w, vec2 v);

#endif // WORLD_H
