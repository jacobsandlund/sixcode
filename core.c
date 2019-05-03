#include "core.h"

// Include all source code in single translation unit
#include "grid.c"
#include "hex-coords.c"
#include "mesh.c"
#include "quad.c"
#include "shader.c"
#include "ui-all.c"
#include "ui-fill.c"
#include "ui-grid.c"
#include "view.c"

#define CORE_HEX_FILL_MIN 1
#define CORE_HEX_FILL_MAX 15

void core_grid_initialize(Grid *g, Quad *quad)
{
	grid_initialize(g, quad);
}

void core_tick(UiAll *ui, View *vw, Grid *g)
{
	ui_all_draw(ui, vw, g);
}

bool core_grid_expand_for_hex(Grid *g, ivec2 h)
{
	Quad expanded;
	quad_expand_for_hex(&expanded, &g->quad, h);

	Quad styles_quad;
	grid_quad_to_styles_quad(&styles_quad, &expanded);

	SizeQuad sq;
	quad_to_storage_size_quad(&sq, &styles_quad);

	if (sq.size.x > UI_GRID_MAX_TEXTURE_SIZE || sq.size.y > UI_GRID_MAX_TEXTURE_SIZE) {
		return false;
	}

	grid_expand_quad(g, &expanded);

	return true;
}

void core_toggle_hex_at_point(UiAll *ui, View *vw, Grid *g, vec2 v)
{
	static u8 style = CORE_HEX_FILL_MIN;

	ivec2 h = hex_coords_round(view_world_to_hex(
			view_screen_to_world(vw, v)));

	bool expanded = false;

	if (!quad_contains(&g->quad, h)) {
		expanded = core_grid_expand_for_hex(g, h);

		if (!expanded) {
			return;
		}
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

	if (expanded) {
		ui_grid_update_styles(&ui->grid, g);
	} else {
		Quad quad = {h, h};
		ui_grid_update_styles_in_quad(&ui->grid, g, &quad);
	}
}
