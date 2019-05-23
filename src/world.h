#ifndef WORLD_H
#define WORLD_H

#include "spacetime.h"
#include "event-queue.h"
#include "grid.h"
#include "viewport.h"

typedef struct {
	Grid grid;
	Viewport viewport;
} World;

void world_initialize(World *w);
void world_terminate(World *w);
void world_load(World *w);
void world_update(World *w, EventQueue *eq);

#endif // WORLD_H
