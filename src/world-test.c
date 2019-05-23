#include "test.h"
#include "world.c"
#include "event-queue.c"
#include "grid.c"
#include "layout.c"
#include "quad.c"
#include "viewport.c"

Test(world_initialize)
{
	World *w = malloc(sizeof *w);

	world_initialize(w);

	_qd(w->grid.quad);
	//=> (-2047, -2047), (2046, 2046)
	_f3(w->viewport.camera);
	//=> 0, 0, 16
	_d(w->viewport.layout.type == LayoutTypeHex);
	//=> 1

	world_terminate(w);

	free(w);
}

Test(world_update)
{
	EventQueue *eq = malloc(sizeof *eq);
	World *w = malloc(sizeof *w);

	event_queue_initialize(eq);
	world_initialize(w);
	w->viewport.size = (float2) {2560, 1440};

	world_update(w, eq);

	world_terminate(w);

	free(eq);
	free(w);
}

Test(world_load)
{
	World *w = malloc(sizeof *w);

	world_initialize(w);
	world_load(w);

	world_terminate(w);
	free(w);
}
