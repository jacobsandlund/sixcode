#include "core.h"

void core_toggle_hex_at_point(Layout *l, Grid *g, Point p)
{
	Hex h = layout_point_to_hex(l, p);

	if (grid_has(g, h)) {
		grid_remove(g, h);
	} else {
		grid_add(g, h, NULL);
	}
}
