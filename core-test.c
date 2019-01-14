#include "test.h"
#include "core.c"
#include "grid.c"
#include "bit-array.c"
#include "point.c"
#include "hex.c"
#include "layout.c"
#include "quad.c"
#include "mesh.c"

#define _hx(h) _dd(h.q, h.r)
#define _pt(p) _gg(p.x, p.y)

TEST(core_toggle_hex_at_point)
{
	Hex h;
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_WITH_DATA);
	Point viewport_size = {.x = 1000, .y = 600};
	Point translation = {.x = -100, .y = -250};
	f64 scale = 20.0;
	Layout *l = layout_create(LAYOUT_POINTY, viewport_size, translation, scale);
	Point p = {.x = 53.0, .y = 127.0};

	h = layout_point_to_hex(l, p);
	_hx(h);
	//=> 1, -4

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

TEST(core_styled_mesh_generate_hexes)
{
	Hex h1 = {.q = 1, .r = 2};
	Hex h2 = {.q = 3, .r = -1};
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_WITH_DATA);
	Point viewport_size = {.x = 1000, .y = 600};
	Point translation = {.x = -100, .y = -250};
	f64 scale = 20.0;
	Layout *l = layout_create(LAYOUT_POINTY, viewport_size, translation, scale);
	i32 hex_capacity = 1;
	i32 style_count = 6;
	StyledMesh *sm = styled_mesh_create(hex_capacity, style_count);
	Mesh *m = &sm->mesh;

	grid_add(g, h1, (void *) 1);
	grid_add(g, h2, (void *) 2);

	core_styled_mesh_generate_hexes(sm, l, g);

	_d(m->hex_count);
	//=> 2
	_pt(m->points[0]);
	//=> 203.923, 230
	_pt(m->points[11]);
	//=> 169.282, 330
	_d(sm->hex_style_indices[0]);
	//=> 2
	_d(sm->hex_style_indices[1]);
	//=> 1
}
