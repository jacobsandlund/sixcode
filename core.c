#include "core.h"

// Include all source code in single translation unit
#include "area.c"
#include "area-quad.c"
#include "grid.c"
#include "hex.c"
#include "mesh.c"
#include "quad.c"
#include "shader.c"
#include "space.c"
#include "ui-all.c"
#include "ui-fill.c"
#include "ui-grid.c"
#include "ui-points.c"
#include "ui-stroke.c"
#include "view.c"

#define CORE_STYLE_MIN 1
#define CORE_STYLE_MAX 15

void core_grids_initialize(Grid *g, AreaGrid *a, Quad *quad)
{
	grid_initialize(g, quad);

	AreaQuad area_quad;
	area_quad_from_quad(&area_quad, quad);
	area_grid_initialize(a, &area_quad);
}

i8 core_grid_expand_for_hex(UiAll *ui, Grid *g, Hex h)
{
	Quad expanded;
	quad_expand_for_hex(&expanded, &g->quad, h);

	Quad styles_quad;
	grid_quad_to_styles_quad(&styles_quad, &expanded);

	SizeQuad sq;
	quad_to_storage_space_size_quad(&sq, &styles_quad);

	if (sq.size.c > UI_GRID_MAX_TEXTURE_SIZE || sq.size.r > UI_GRID_MAX_TEXTURE_SIZE) {
		return 0;
	}

	grid_expand_quad(g, &expanded);
	ui_grid_update_styles(&ui->grid, g);

	return 1;
}

void core_toggle_hex_at_point(UiAll *ui, View *vw, Grid *g, vec2 v)
{
	static u8 style = CORE_STYLE_MIN;

	Hex h = space_hex_round(space_world_to_hex(
			space_screen_to_world(vw, v)));

	if (!quad_contains(&g->quad, h)) {
		if (!core_grid_expand_for_hex(ui, g, h)) {
			return;
		}
	}

	if (grid_get(g, h)) {
		grid_clear(g, h);
	} else {
		grid_set(g, h, style);

		++style;
		if (style > CORE_STYLE_MAX) {
			style = CORE_STYLE_MIN;
		}
	}

	Quad quad = {h, h};
	ui_grid_update_styles_in_quad(&ui->grid, g, &quad);
	ui_all_draw(ui, vw, g);
}

//void core_area_grid_expand_for_quad(AreaGrid *a, Quad *quad)
//{
//	Quad expanded;
//	quad_expand_for_quad(&expanded, &a->quad, quad);
//	area_grid_expand_quad(a, &expanded);
//}
//
//void core_add_area(UiAll *ui, View *vw, AreaGrid *a, vec2 v1, vec2 v2)
//{
//	Quad quad;
//	view_screen_points_to_quad(vw, &quad, v1, v2);
//
//	if (!quad_contains_quad(&a->quad, &quad)) {
//		core_area_grid_expand_for_quad(a, &quad);
//	}
//
//	AreaQuad aq;
//	area_quad_from_quad(&aq, &quad);
//
//	area_grid_add(a, &aq);
//	ui_all_draw(ui, vw, g);
//}
