#include "core.h"

#define CORE_STYLE_COUNT 4

static i32 core_style_index = 0;

void core_toggle_hex_at_point(Layout *l, Grid *g, Point p)
{
	Hex h = layout_point_to_hex(l, p);

	if (grid_has(g, h)) {
		grid_remove(g, h);
	} else {
		grid_add(g, h, (void *) core_style_index);

		++core_style_index;
		if (core_style_index >= CORE_STYLE_COUNT) {
			core_style_index = 0;
		}
	}
}

i32 core_styled_mesh_style_fn(void *context, Hex h, void *data)
{
	(void) context;
	(void) h;

	return (i32) data;
}

void core_styled_mesh_generate_hexes(StyledMesh *sm, Layout *l, Grid *g)
{
	styled_mesh_clear(sm);
	styled_mesh_add_hexes(sm, l, g, NULL, core_styled_mesh_style_fn);
}
