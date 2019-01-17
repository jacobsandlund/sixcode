#include "test.h"
#include "core.c"
#include "grid.c"
#include "bit-array.c"
#include "point.c"
#include "hex.c"
#include "layout.c"
#include "quad.c"
#include "mesh.c"

#define _hx(h) _dd(h.c, h.r)
#define _pt(p) _gg(p.x, p.y)

TEST(core_toggle_hex_at_point)
{
	Hex h;
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_WITH_DATA);
	Point viewport_size = {.x = 1000, .y = 600};
	Point translation = {.x = -100, .y = -250};
	f64 scale = 20.0;
	Layout *l = layout_create(viewport_size, translation, scale);
	Point p = {.x = 53.0, .y = 127.0};

	h = layout_point_to_hex(l, p);
	_hx(h);
	//=> -2, -4

	core_toggle_hex_at_point(l, g, p);

	_d(g->set_count);
	//=> 1
	_d(grid_has(g, h));
	//=> 1
	_d((i32) grid_get(g, h));
	//=> 0

	core_toggle_hex_at_point(l, g, p);
	_d(grid_has(g, h));
	//=> 0

	// Increment style index
	core_toggle_hex_at_point(l, g, p);
	_d((i32) grid_get(g, h));
	//=> 1

	grid_destroy(g);
	layout_destroy(l);
}
