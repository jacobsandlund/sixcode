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
	Mesh *m = mesh_create(32, 6);

	_d(m->points != NULL);
	//=> 1
	_d(m->hex_capacity);
	//=> 32
	_d(m->hex_count);
	//=> 0

	mesh_destroy(m);
}

TEST(mesh_clear)
{
	Mesh m = {.hex_count = 32};

	mesh_clear(&m);

	_d(m.hex_count);
	//=> 0
}

TEST(mesh_ensure_hex_capacity)
{
	Mesh *m = mesh_create(4, 6);

	_d(m->hex_capacity);
	//=> 4

	m->hex_count = 1;
	m->points[5].x = 100.0;
	m->points[5].y = 463.7;

	mesh_ensure_hex_capacity(m, 3, 6);

	_d(m->hex_capacity);
	//=> 3
	_d(m->points_per_hex);
	//=> 6
	_pt(m->points[5]);
	//=> 100, 463.7

	// No new capacity needed, but switch points per hex
	m->hex_count = 2;
	mesh_ensure_hex_capacity(m, 12, 2);

	_pt(m->points[5]);
	//=> 100, 463.7

	_d(m->hex_capacity);
	//=> 12
	_d(m->points_per_hex);
	//=> 2

	m->hex_count = 6;
	mesh_ensure_hex_capacity(m, 60, 1);

	_d(m->hex_capacity);
	//=> 96
	_pt(m->points[5]);
	//=> 100, 463.7
	_d(m->points_per_hex);
	//=> 1

	mesh_destroy(m);
}

TEST(mesh_add_hexes)
{
	Hex h1 = {.q = 1, .r = 2};
	Hex h2 = {.q = 3, .r = -1};
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
	Point viewport_size = {.x = 1000, .y = 600};
	Point translation = {.x = -100, .y = -250};
	f64 scale = 20.0;
	Layout *l = layout_create(LAYOUT_POINTY, viewport_size, translation, scale);
	Mesh *m = mesh_create(1, 6);

	grid_add(g, h1, NULL);

	_d(g->quad.min.q);
	//=> 1
	_d(g->quad.min.r);
	//=> 2
	_d(g->set == NULL);
	//=> 0
	mesh_add_hexes(m, l, g);

	_d(m->hex_count);
	//=> 1
	_d(m->hex_capacity);
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
	grid_remove(g, h1);
	grid_add(g, h2, NULL);
	mesh_add_hexes(m, l, g);

	_d(m->hex_capacity);
	//=> 2
	_d(m->hex_count);
	//=> 2

	_pt(m->points[0]);
	//=> 186.603, 320
	_pt(m->points[6]);
	//=> 203.923, 230
	_pt(m->points[11]);
	//=> 186.603, 240

	grid_destroy(g);
	layout_destroy(l);
	mesh_destroy(m);
}

TEST(mesh_add_points_at_hexes)
{
	Hex h1 = {.q = 1, .r = 2};
	Hex h2 = {.q = 3, .r = -1};
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
	Point viewport_size = {.x = 1000, .y = 600};
	Point translation = {.x = -100, .y = -250};
	f64 scale = 20.0;
	Layout *l = layout_create(LAYOUT_POINTY, viewport_size, translation, scale);
	i32 points_per_hex = 2;
	Mesh *m = mesh_create(1, points_per_hex);

	grid_add(g, h1, NULL);
	grid_add(g, h2, NULL);

	mesh_add_points_at_hexes(m, l, g);

	_d(m->hex_count);
	//=> 2
	_d(m->hex_capacity);
	//=> 2
	_d(m->points_per_hex);
	//=> 1

	_pt(m->points[0]);
	//=> 169.282, 202.679
	_pt(m->points[1]);
	//=> 151.962, 292.679

	grid_destroy(g);
	layout_destroy(l);
	mesh_destroy(m);
}

TEST(styled_mesh_create_and_destroy)
{
	StyledMesh *sm = styled_mesh_create(32, 6, 4);
	Mesh *m = &sm->mesh;

	_d(m->points != NULL);
	//=> 1
	_d(m->hex_capacity);
	//=> 32
	_d(m->hex_count);
	//=> 0
	_d(sm->hex_style_indices != NULL);
	//=> 1
	_d(sm->hex_count_for_style != NULL);
	//=> 1
	_d(sm->style_count);
	//=> 4
	_d(sm->hex_count_for_style[3]);
	//=> 0

	styled_mesh_destroy(sm);
}

TEST(styled_mesh_clear)
{
	StyledMesh *sm = styled_mesh_create(32, 6, 4);
	Mesh *m = &sm->mesh;

	m->hex_count = 30;
	sm->style_count = 4;
	sm->hex_count_for_style[0] = 5;
	sm->hex_count_for_style[3] = 25;

	styled_mesh_clear(sm);

	_d(m->hex_count);
	//=> 0
	_d(sm->style_count);
	//=> 4
	_d(sm->hex_count_for_style[0]);
	//=> 0
	_d(sm->hex_count_for_style[3]);
	//=> 0

	styled_mesh_destroy(sm);
}

TEST(styled_mesh_ensure_hex_capacity)
{
	StyledMesh *sm = styled_mesh_create(4, 6, 33);
	Mesh *m = &sm->mesh;

	_d(m->hex_capacity);
	//=> 4

	m->hex_count = 2;
	m->points[5].x = 100.0;
	m->points[5].y = 463.7;
	sm->hex_style_indices[1] = 29;

	styled_mesh_ensure_hex_capacity(sm, 7, 6);

	_d(m->hex_capacity);
	//=> 8
	_pt(m->points[5]);
	//=> 100, 463.7
	_d(sm->hex_style_indices[1]);
	//=> 29

	mesh_ensure_hex_capacity(m, 60, 1);

	_d(m->hex_capacity);
	//=> 64
	_pt(m->points[5]);
	//=> 100, 463.7
	_d(sm->hex_style_indices[1]);
	//=> 29

	styled_mesh_destroy(sm);
}

static i32 test_styled_mesh_style_fn(void *context, Hex h, void *data)
{
	(void) h;

	return (i32) context + (i32) data;
}

TEST(styled_mesh_add_hexes)
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
	i32 style_context = 3;
	StyledMesh *sm = styled_mesh_create(hex_capacity, 6, style_count);
	Mesh *m = &sm->mesh;

	grid_add(g, h1, (void *) 1);

	styled_mesh_add_hexes(sm, l, g, (void *) style_context, test_styled_mesh_style_fn);

	_d(m->hex_count);
	//=> 1

	_pt(m->points[0]);
	//=> 186.603, 320
	_pt(m->points[5]);
	//=> 169.282, 330

	_d(sm->hex_style_indices[0]);
	//=> 4
	_d(sm->hex_count_for_style[4]);
	//=> 1

	grid_remove(g, h1);
	grid_add(g, h2, (void *) 2);

	// Expand hex capacity
	styled_mesh_add_hexes(sm, l, g, (void *) style_context, test_styled_mesh_style_fn);

	_d(m->hex_count);
	//=> 2
	_d(m->hex_capacity);
	//=> 2

	_pt(m->points[5]);
	//=> 169.282, 330
	_pt(m->points[6]);
	//=> 203.923, 230
	_pt(m->points[11]);
	//=> 186.603, 240

	_d(sm->hex_style_indices[0]);
	//=> 4
	_d(sm->hex_style_indices[1]);
	//=> 5
	_d(sm->hex_count_for_style[4]);
	//=> 1
	_d(sm->hex_count_for_style[5]);
	//=> 1

	styled_mesh_destroy(sm);
	grid_destroy(g);
	layout_destroy(l);
}

TEST(styled_mesh_add_points_at_hexes)
{
	Hex h1 = {.q = 1, .r = 2};
	Hex h2 = {.q = 3, .r = -1};
	Grid *g = grid_create(HEX_ZERO, 0.0, GRID_NO_DATA);
	Point viewport_size = {.x = 1000, .y = 600};
	Point translation = {.x = -100, .y = -250};
	f64 scale = 20.0;
	Layout *l = layout_create(LAYOUT_POINTY, viewport_size, translation, scale);
	i32 style_count = 6;
	i32 style_context = 3;
	i32 points_per_hex = 2;
	StyledMesh *sm = styled_mesh_create(1, points_per_hex, style_count);
	Mesh *m = &sm->mesh;

	grid_add(g, h1, (void *) 1);
	grid_add(g, h2, (void *) 2);

	styled_mesh_add_points_at_hexes(sm, l, g, (void *) style_context, test_styled_mesh_style_fn);

	_d(m->hex_count);
	//=> 2
	_d(m->hex_capacity);
	//=> 2
	_d(m->points_per_hex);
	//=> 1

	_pt(m->points[0]);
	//=> 169.282, 202.679
	_pt(m->points[1]);
	//=> 151.962, 292.679
	_d(sm->hex_style_indices[0]);
	//=> 3
	_d(sm->hex_style_indices[1]);
	//=> 3

	grid_destroy(g);
	layout_destroy(l);
	styled_mesh_destroy(sm);
}
