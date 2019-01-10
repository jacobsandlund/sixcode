#include "test.h"
#include "core.c"
#include "grid.c"
#include "bit-array.c"
#include "point.c"
#include "hex.c"
#include "layout.c"
#include "quad.c"

#define _hx(h) _dd(h.q, h.r)

TEST(core_toggle_hex_at_point)
{
	Hex h;
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
	f64 scale = 20.0;
	Point origin = {.x = 100, .y = 250};
	Layout *l = layout_create(LAYOUT_POINTY, scale, origin);
	Point p = {.x = 53.0, .y = 127.0};

	h = layout_point_to_hex(l, p);
	_hx(h);
	//=> 1, -4

	core_toggle_hex_at_point(l, g, p);

	_d(g->set_count);
	//=> 1
	_d(grid_has(g, h));
	//=> 1

	core_toggle_hex_at_point(l, g, p);
	_d(grid_has(g, h));
	//=> 0

	grid_destroy(g);
	layout_destroy(l);
}
