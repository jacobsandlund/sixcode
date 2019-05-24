#include "test.h"
#include "render-loop.c"
#include "event-queue.c"
#include "grid.c"
#include "layout.c"
#include "quad.c"
#include "renderer-mock.c"
#include "viewport.c"
#include "world.c"

Test(render_loop_one_iteration)
{
	Renderer *r = malloc(sizeof *r);
	EventQueue *eq = malloc(sizeof *eq);
	World *w = malloc(sizeof *w);

	renderer_mock_init(r);
	event_queue_init(eq);
	world_init(w);
	world_load(w);

	render_loop_one_iteration(r, eq, w);

	RendererMock *rm = renderer_mock(r);
	_d(rm->num_render_calls);
	//=> 1

	renderer_mock_destroy(r);
	world_destroy(w);

	free(eq);
	free(r);
	free(w);
}
