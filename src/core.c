#include "core.h"

void core_loop_tick(Renderer *r, View *vw, World *w)
{
	world_update(w, vw);
	renderer_render(r, vw, w);
}
