#include "core.h"


// Include all source code in single translation unit
#include "bit-array.c"
#include "grid.c"
#include "hex.c"
#include "matrix.c"
#include "mesh.c"
#include "quad.c"
#include "ui.c"
#include "view.c"

#define CORE_STYLE_MIN 1
#define CORE_STYLE_MAX 15

void core_toggle_hex_at_point(Ui *ui, View *vw, Grid *g, vec2 v)
{
	static u8 style = CORE_STYLE_MIN;

	Hex h = view_point_to_hex(vw, v);

	if (quad_contains(&g->quad, h)) {
		if (grid_has(g, h)) {
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
		ui_draw(ui, vw);
	}
}
