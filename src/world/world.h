#ifndef World_h
#define World_h

#include "spacetime.h"
#include "engine/event-queue.h"
#include "model/grid.h"
#include "render/viewport.h"

typedef struct {
	Grid grid;
	Viewport viewport;
} World;

void world_init(World *w);
void world_destroy(World *w);
void world_load(World *w);
void world_update(World *w, EventQueue *eq);

#endif // World_h
