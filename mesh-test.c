#include "test.h"
#include "mesh.c"
#include "hex.c"
#include "point.c"
#include "layout.c"
#include "grid.c"
#include "bit-array.c"
#include "quad.c"

#define _pt(p) _gg(p.x, p.y)

TEST(mesh_create_and_destroy)
{
	Mesh *m = mesh_create(32);

	_d(m->points != NULL);
	//=> 1
	_d(m->hex_count);
	//=> 0
	_d(m->hex_capacity);
	//=> 32

	mesh_destroy(m);
}

TEST(mesh_generate_hexes)
{
	Hex h1 = {.q = 1, .r = 2};
	Hex h2 = {.q = 3, .r = -1};
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
	Point viewport_size = {.x = 1000, .y = 600};
	Point translation = {.x = -100, .y = -250};
	f64 scale = 20.0;
	Layout *l = layout_create(LAYOUT_POINTY, viewport_size, translation, scale);
	Mesh *m = mesh_create(1);

	grid_add(g, h1, NULL);

	_d(g->quad.min.q);
	//=> 1
	_d(g->quad.min.r);
	//=> 2
	_d(g->set == NULL);
	//=> 0
	_d(mesh_generate_hexes(m, l, g));
	//=> 1

	_d(m->hex_count);
	//=> 1

	_pt(m->points[0]);
	//=> 186.603, 320
	_pt(m->points[1]);
	//=> 186.603, 300
	_pt(m->points[2]);
	//=> 169.282, 290
	_pt(m->points[3]);
	//=> 151.962, 300
	_pt(m->points[4]);
	//=> 151.962, 320
	_pt(m->points[5]);
	//=> 169.282, 330

	// Expand points capacity
	grid_add(g, h2, NULL);
	_d(mesh_generate_hexes(m, l, g));
	//=> 2
	_d(m->hex_capacity);
	//=> 2

	_pt(m->points[0]);
	//=> 203.923, 230
	_pt(m->points[6]);
	//=> 186.603, 320
	_pt(m->points[11]);
	//=> 169.282, 330

	grid_destroy(g);
	layout_destroy(l);
	mesh_destroy(m);
}
