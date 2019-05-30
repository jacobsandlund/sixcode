#include "test.h"
#include "world/world.c"
#include "world/grid.c"
#include "math/quad.c"

Test(world_init)
{
	World *w = tmalloc(sizeof *w);

	world_init(w);

	_qd(w->grid.quad);
	//=> (-2047, -2047), (2046, 2046)

	world_destroy(w);
}

Test(world_load)
{
	World *w = tmalloc(sizeof *w);

	world_init(w);
	world_load_random(w, 256);

	world_destroy(w);
}
