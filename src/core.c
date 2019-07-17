#include "core.h"

// Include all source code in single translation unit
#include "grid.c"
#include "mesh.c"
#include "quad.c"
#include "shader.c"
#include "texture.c"
#include "ui.c"
#include "view.c"

#define CORE_HEX_FILL_MIN 1
#define CORE_HEX_FILL_MAX 15

void core_grid_initialize(Grid *g)
{
	grid_initialize(g);
}

void core_tick(Ui *ui, View *vw, Grid *g)
{
	ui_draw(ui, vw, g);
}

void core_toggle_hex_at_point(Ui *ui, View *vw, Grid *g, vec2 v)
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
	texture_update_in_quad(&ui->grid_styles_texture, g, &quad);
}
