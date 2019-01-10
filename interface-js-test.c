#include "test.h"
#include "interface-js.c"
#include "core.c"
#include "grid.c"
#include "bit-array.c"
#include "point.c"
#include "hex.c"
#include "layout.c"
#include "quad.c"
#include "mesh.c"

#define _pt(p) _gg(p.x, p.y)
#define _hx(h) _dd(h.q, h.r)

TEST(js_layout_create)
{
	Layout *l = js_layout_create(10.0, 100.0, 200.0);
	_g(l->scale);
	//=> 10
	_pt(l->origin);
	//=> 100, 200

	layout_destroy(l);
}

TEST(js_layout_set_scale)
{
	Layout l;
	js_layout_set_scale(&l, 35.5);
	_g(l.scale);
	//=> 35.5
}

TEST(js_layout_set_origin)
{
	Layout l;
	js_layout_set_origin(&l, -400.0, 375.2);
	_pt(l.origin);
	//=> -400, 375.2
}

TEST(js_grid_create)
{
	Grid *g = js_grid_create();
	
	_d(g->with_data);
	//=> 0
	_hx(g->extra_capacity);
	//=> 0, 0
	_g(g->extra_capacity_growth_factor);
	//=> 0

	grid_destroy(g);
}

TEST(js_mesh_create_and_points)
{
	Mesh *m = js_mesh_create(7);

	_d(m->hex_capacity);
	//=> 7

	_d(js_mesh_points(m) == m->points);
	//=> 1

	mesh_destroy(m);
}

TEST(js_core_toggle_hex_at_point)
{
	Hex h;
	Grid *g = js_grid_create();
	f64 scale = 20.0;
	f64 origin_x = 100;
	f64 origin_y = 250;
	Layout *l = js_layout_create(scale, origin_x, origin_y);
	f64 x = 53.0;
	f64 y = 127.0;
	Point p = {.x = x, .y = y};

	h = layout_point_to_hex(l, p);
	_hx(h);
	//=> 1, -4

	js_core_toggle_hex_at_point(l, g, x, y);

	_d(g->set_count);
	//=> 1
	_d(grid_has(g, h));
	//=> 1

	grid_destroy(g);
	layout_destroy(l);
}
