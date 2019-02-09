#include "core.h"


// Include all source code in single translation unit
#include "draw.c"
#include "grid.c"
#include "hex.c"
#include "matrix.c"
#include "mesh.c"
#include "quad.c"
#include "shader.c"
#include "ui.c"
#include "view.c"

#define CORE_STYLE_MIN 1
#define CORE_STYLE_MAX 15

void core_grid_initialize(Grid *g)
{
	//Quad quad = {{-UI_MAX_TEXTURE_SIZE + 2, -UI_MAX_TEXTURE_SIZE / 2 + 1}, {UI_MAX_TEXTURE_SIZE - 3, UI_MAX_TEXTURE_SIZE / 2 - 2}};
	Quad quad = {{2, 1}, {125, 62}};
	grid_initialize(g, &quad);
}

i8 core_grid_expand_for_hex(Ui *ui, Grid *g, Hex h)
{
	Quad expanded;
	quad_expand_quad(&expanded, &g->quad, h);

	Quad styles_quad;
	grid_styles_quad_from_quad(&styles_quad, &expanded);

	StorageQuad sq;
	storage_quad_from_quad(&sq, &styles_quad);

	if (sq.size.c > UI_MAX_TEXTURE_SIZE || sq.size.r > UI_MAX_TEXTURE_SIZE) {
		return 0;
	}

	grid_expand_quad(g, &expanded);
	ui_update_styles(ui, g);

	return 1;
}

void core_toggle_hex_at_point(Ui *ui, View *vw, Grid *g, vec2 v)
{
	static u8 style = CORE_STYLE_MIN;

	Hex h = hex_round(view_point_to_vec_hex(vw, v));

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
	ui_update_styles_in_quad(ui, g, &quad);
	draw(ui, vw, g);
}
