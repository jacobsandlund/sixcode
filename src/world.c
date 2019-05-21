#include "world.h"
#include "layout.h"

#define WORLD_HEX_FILL_MIN 1
#define WORLD_HEX_FILL_MAX 15

void world_initialize(World *w)
{
	grid_initialize(&w->grid);

	Viewport *vp = &w->viewport;
	vp->camera = (float3) {0.0, 0.0, 16.0};
	layout_kind(&vp->layout, LAYOUT_HEX);
}

void world_terminate(World *w)
{
	grid_terminate(&w->grid);
}

void world_load(World *w)
{
	(void) w;
}

void world_update(World *w, View *vw)
{
	w->viewport.size = vw->viewport_size;
}

/*
void world_toggle_hex_at_point(World *w, vec2 v)
{
	static u8 style = WORLD_HEX_FILL_MIN;

	ivec2 h = camera_world_vector_round(c, camera_screen_to_world_vector(c, v));

	if (!quad_contains(&g->quad, h)) {
		return;
	}

	if (grid_get(g, h)) {
		grid_clear(g, h);
	} else {
		grid_set(g, h, style);

		++style;
		if (style > WORLD_HEX_FILL_MAX) {
			style = WORLD_HEX_FILL_MIN;
		}
	}

	Quad quad = {h, h};
	texture_update_in_quad(&ui->grid_styles_texture, g, &quad);
}
*/
