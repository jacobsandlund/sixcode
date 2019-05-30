#ifndef World_h
#define World_h

#include "spacetime.h"
#include "world/grid.h"

typedef struct {
	WorldGrid grid;
} World;

void world_init(World *w);
void world_destroy(World *w);
void world_load_random(World *w, i64 count);
// TODO: void world_toggle_hex_at_point(World *w, vec2 v);

#endif // World_h
