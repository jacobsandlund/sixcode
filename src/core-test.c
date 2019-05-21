#include "core.c"
#include "test.h"
#include "camera.c"
#include "grid.c"
#include "layout.c"
#include "quad.c"
#include "renderer-mock.c"
#include "view-mock.c"
#include "viewport.c"
#include "world.c"

TEST(core_loop_tick)
{
	World *w = malloc(sizeof *w);
	View *vw = malloc(sizeof *vw);
	Renderer *r = malloc(sizeof *r);

	float2 viewport_size = {2560, 1440};

	world_initialize(w);
	view_mock_initialize(vw, viewport_size);
	renderer_mock_initialize(r);
	world_load(w);

	core_loop_tick(r, vw, w);

	RendererMock *rm = renderer_mock(r);
	_d(rm->num_render_calls);
	//=> 1

	world_terminate(w);
	renderer_mock_terminate(r);
	view_mock_terminate(vw);

	free(w);
	free(r);
	free(vw);
}
