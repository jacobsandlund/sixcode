#include "engine/render-loop.h"

void render_loop_one_iteration(Renderer *r, EventQueue *eq, World *w)
{
	world_update(w, eq);
	renderer_render(r, w);
}
