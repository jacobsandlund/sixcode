#include "test.h"
#include "hex.c"
#include "view.c"
#include "matrix.c"

#define _v2(v) _gg(v.x, v.y)

TEST(view_initialize)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {100, 250};
	f32 scale = 20.0;

	View *vw = malloc(sizeof *vw);
	view_initialize(vw, viewport_size, translation, scale);

	_v2(vw->viewport_size);
	//=> 1000, 600
	_v2(vw->translation);
	//=> 100, 250
	_g(vw->scale);
	//=> 20
	_gggg(vw->view_matrix.m[0][0], vw->view_matrix.m[0][1], vw->view_matrix.m[3][0], vw->view_matrix.m[3][3]);
	//=> 0.001, 0, -0.01, 0.05

	free(vw);
}

TEST(view_update_matrix)
{
	vec2 viewport_size = {1000, 800};
	vec2 translation = {200, -400};
	f32 scale = 10.0;

	View *vw = malloc(sizeof *vw);
	view_initialize(vw, viewport_size, translation, scale);

	view_update_matrix(vw);

	mat4 m = vw->view_matrix;

	_gggg(m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3]);
	//=> 0.001, 0, 0, 0
	_gggg(m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3]);
	//=> 0, 0.00125, 0, 0
	_gggg(m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3]);
	//=> 0, 0, 0, 0
	_gggg(m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);
	//=> -0.04, -0.1, 0, 0.1

	free(vw);
}

TEST(view_zoom_at_point)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {500, -2000};
	f32 scale = 20.0;

	View *vw = malloc(sizeof *vw);
	view_initialize(vw, viewport_size, translation, scale);

	vec2 v = viewport_size;
	view_zoom_at_point(vw, v, 30.0);
	_v2(vw->translation);
	//=> 1000, -2850
	_g(vw->scale);
	//=> 30
	_gg(vw->view_matrix.m[3][0], vw->view_matrix.m[3][3]);
	//=> -0.0666667, 0.0333333

	v = (vec2) {300, 400};
	view_zoom_at_point(vw, v, 10.0);
	_v2(vw->translation);
	//=> 466.667, -1016.67
	_g(vw->scale);
	//=> 10
	_gg(vw->view_matrix.m[3][0], vw->view_matrix.m[3][3]);
	//=> -0.0933333, 0.1

	free(vw);
}

TEST(view_resize)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {500, -2000};
	f32 scale = 20.0;
	vec2 v = {300, 400};

	View *vw = malloc(sizeof *vw);
	view_initialize(vw, viewport_size, translation, scale);

	view_resize(vw, v);

	_v2(vw->viewport_size);
	//=> 300, 400
	_gggg(vw->view_matrix.m[0][0], vw->view_matrix.m[1][1], vw->view_matrix.m[3][0], vw->view_matrix.m[3][3]);
	//=> 0.00333333, 0.0025, -0.166667, 0.05

	free(vw);
}

TEST(view_point_to_hex)
{
	View vw = {
		.viewport_size = {1000, 600},
		.translation = {-100, -200},
		.scale = 10,
	};
	vec2 v1 = {125.981, 275};
	vec2 v2 = {980.3, 540};
	Hex h;

	h = view_point_to_hex(&vw, v1);
	_dd(h.c, h.r);
	//=> -110, -30

	h = view_point_to_hex(&vw, v2);
	_dd(h.c, h.r);
	//=> 87, 5
}
