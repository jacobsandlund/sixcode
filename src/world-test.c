#include "world.c"
#include "test.h"
#include "camera.c"
#include "grid.c"
#include "layout.c"
#include "quad.c"
#include "view-mock.c"
#include "viewport.c"

TEST(world_initialize)
{
	World *w = malloc(sizeof *w);

	world_initialize(w);

	_qd(w->grid.quad);
	//=> (-2047, -2047), (2046, 2046)
	_f3(w->viewport.camera);
	//=> 0, 0, 16
	_d(w->viewport.layout.type == LAYOUT_TYPE_HEX);
	//=> 1

	world_terminate(w);

	free(w);
}

TEST(world_update)
{
	World *w = malloc(sizeof *w);
	View *vw = malloc(sizeof *vw);

	float2 viewport_size = {2560, 1440};

	world_initialize(w);
	view_mock_initialize(vw, viewport_size);

	world_update(w, vw);

	world_terminate(w);
	view_mock_terminate(vw);

	free(w);
	free(vw);
}

TEST(world_load)
{
	World *w = malloc(sizeof *w);

	world_initialize(w);
	world_load(w);

	world_terminate(w);
	free(w);
}
