#include "core.h"

// Include all source code in single translation unit
#include "grid.c"
#include "mesh.c"
#include "quad.c"
#include "shader.c"
#include "texture.c"
#include "ui-all.c"
#include "ui-fill.c"
#include "view.c"

#define CORE_HEX_FILL_MIN 1
#define CORE_HEX_FILL_MAX 15

void core_grid_initialize(Grid *g)
{
	grid_initialize(g);
}

void core_tick(UiAll *ui, View *vw, Grid *g)
{
	ui_all_draw(ui, vw, g);
}

void core_toggle_hex_at_point(UiAll *ui, View *vw, Grid *g, vec2 v)
{
	static u8 style = CORE_HEX_FILL_MIN;

	ivec2 h = view_world_round(vw, view_screen_to_world(vw, v));

	if (!quad_contains(&g->quad, h)) {
		return;
	}

	if (grid_get(g, h)) {
		grid_clear(g, h);
	} else {
		grid_set(g, h, style);

		++style;
		if (style > CORE_HEX_FILL_MAX) {
			style = CORE_HEX_FILL_MIN;
		}
	}

	Quad quad = {h, h};
	texture_update_in_quad(&ui->fill.grid_styles_texture, g, &quad);
}
