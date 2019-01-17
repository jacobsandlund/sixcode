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
#define _hx(h) _dd(h.c, h.r)

TEST(js_layout_create)
{
	f64 width = 1000.0;
	f64 height = 600.0;
	f64 translation_x = 100;
	f64 translation_y = 250;
	f64 scale = 20.0;
	Layout *l = js_layout_create(width, height, translation_x, translation_y, scale);

	_pt(l->viewport_size);
	//=> 1000, 600
	_pt(l->translation);
	//=> 100, 250
	_g(l->scale);
	//=> 20

	layout_destroy(l);
}

TEST(js_layout_translate_by_delta)
{
	Layout l = {.translation = {.x = 500.0, .y = 700.0}};
	js_layout_translate_by_delta(&l, -400.0, 375.2);
	_pt(l.translation);
	//=> 100, 1075.2
}

TEST(js_layout_zoom_at_point)
{
	f64 width = 1000.0;
	f64 height = 600.0;
	f64 scale = 20.0;
	Layout *l = js_layout_create(width, height, 0.0, 0.0, scale);
	f64 x = 300.0;
	f64 y = 400.0;

	js_layout_zoom_at_point(l, x, y, 30.0);
	_pt(l->translation);
	//=> 150, 200
	_g(l->scale);
	//=> 30

	layout_destroy(l);
}

TEST(js_layout_resize_viewport)
{
	Layout l;
	js_layout_resize_viewport(&l, 1440, 1080);
	_pt(l.viewport_size);
	//=> 1440, 1080
}

TEST(js_grid_create)
{
	Grid *g = js_grid_create();
	
	_d(g->with_data);
	//=> 1
	_hx(g->extra_capacity);
	//=> 0, 0
	_g(g->extra_capacity_growth_factor);
	//=> 0

	grid_destroy(g);
}

TEST(js_mesh_create_and_points)
{
	Mesh *m = js_mesh_create();

	_d(m->point_capacity);
	//=> 0

	mesh_ensure_capacity(m, 1, 6);

	_d(js_mesh_points(m) == m->points);
	//=> 1

	mesh_destroy(m);
}

TEST(js_core_toggle_hex_at_point)
{
	Hex h;
	Grid *g = js_grid_create();
	f64 width = 1000.0;
	f64 height = 600.0;
	f64 translation_x = 100;
	f64 translation_y = 250;
	f64 scale = 20.0;
	Layout *l = js_layout_create(width, height, translation_x, translation_y, scale);
	f64 x = 53.0;
	f64 y = 127.0;
	Point p = {.x = x, .y = y};

	h = layout_point_to_hex(l, p);
	_hx(h);
	//=> 9, 13

	js_core_toggle_hex_at_point(l, g, x, y);

	_d(g->set_count);
	//=> 1
	_d(grid_has(g, h));
	//=> 1

	grid_destroy(g);
	layout_destroy(l);
}
