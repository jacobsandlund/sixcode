#include "test.h"
#include "hex.c"
#include "view.c"
#include "matrix.c"

#define _v2(v) _gg(v.x, v.y)
#define _hx(h) _dd(h.c, h.r)

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

	free(vw);
}

TEST(view_zoom_at_point)
{
	vec2 viewport_size = {1000, 600};
	vec2 translation = {0, 0};
	f32 scale = 20.0;

	View *vw = malloc(sizeof *vw);
	view_initialize(vw, viewport_size, translation, scale);

	vec2 v = viewport_size;
	view_zoom_at_point(vw, v, 30.0);
	_v2(vw->translation);
	//=> 500, 300
	_g(vw->scale);
	//=> 30

	v = vw->translation;
	view_zoom_at_point(vw, v, 10.0);
	_v2(vw->translation);
	//=> -166.667, -100
	_g(vw->scale);
	//=> 10

	v.x = 300; v.y = 400;
	view_zoom_at_point(vw, v, 10.0);
	_v2(vw->translation);
	//=> -166.667, -100
	_g(vw->scale);
	//=> 10

	free(vw);
}

TEST(view_resize)
{
	View vw = {.viewport_size = {200, 100}};
	vec2 v = {300, 400};
	view_resize(&vw, v);

	_v2(vw.viewport_size);
	//=> 300, 400
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

TEST(view_hex_to_point)
{
	vec2 translation = {-100, -200};
	View vw = {.scale = 10, .translation = translation};
	Hex h = {.c = 3, .r = 5};
	vec2 v = view_hex_to_point(&vw, h);

	_v2(v);
	//=> 125.981, 275
	_("%.14f", v.x);
	//=> 125.98075866699219

	f32 sqrt_3 = sqrt(3);
	_f(sqrt_3 / 2.0 * 3);
	//=> 2.598076
	_f((sqrt_3 / 2.0 * 3) * 10 + 100);
	//=> 125.980762
	_f(3.0 / 2.0 * 5);
	//=> 7.500000
	_f((3.0 / 2.0 * 5) * 10 + 200);
	//=> 275.000000
}

TEST(view_point_to_float_hex)
{
	vec2 translation = {-100, -200};
	View vw = {.scale = 10, .translation = translation};
	vec2 v = {125.981, 275};
	FloatHex fh = view_point_to_float_hex(&vw, v);

	_gg(fh.c, fh.r);
	//=> 3.00003, 5

	_hx(hex_round(fh));
	//=> 3, 5
}

TEST(point_to_float_hex_roundtrips)
{
	f32 scale = 10;
	vec2 translation = {-35, -71};
	View vw = {.scale = scale, .translation = translation};
	Hex h = {.c = 3, .r = 5};

	_d(hex_equal(hex_round(view_point_to_float_hex(&vw, view_hex_to_point(&vw, h))), h));
	//=> 1
}
