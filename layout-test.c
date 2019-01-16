#include "test.h"
#include "hex.c"
#include "point.c"
#include "layout.c"

#define _pt(p) _gg(p.x, p.y)
#define _hx(h) _dd(h.c, h.r)

TEST(layout_create_and_destroy)
{
	Point viewport_size = {.x = 1000, .y = 600};
	Point translation = {.x = 100, .y = 250};
	f64 scale = 20.0;
	Layout *l = layout_create(viewport_size, translation, scale);

	_pt(l->viewport_size);
	//=> 1000, 600
	_pt(l->translation);
	//=> 100, 250
	_g(l->scale);
	//=> 20

	layout_destroy(l);
}

TEST(layout_zoom_at_point)
{
	Point viewport_size = {.x = 1000, .y = 600};
	Point translation = {.x = 0, .y = 0};
	f64 scale = 20.0;
	Layout *l = layout_create(viewport_size, translation, scale);
	Point p;

	p = viewport_size;
	layout_zoom_at_point(l, p, 30.0);
	_pt(l->translation);
	//=> 500, 300
	_g(l->scale);
	//=> 30

	p = l->translation;
	layout_zoom_at_point(l, p, 10.0);
	_pt(l->translation);
	//=> -166.667, -100
	_g(l->scale);
	//=> 10

	p.x = 300; p.y = 400;
	layout_zoom_at_point(l, p, 10.0);
	_pt(l->translation);
	//=> -166.667, -100
	_g(l->scale);
	//=> 10

	layout_destroy(l);
}

TEST(layout_hex_to_point)
{
	Point translation = {.x = -100, .y = -200};
	Layout l = {.scale = 10, .translation = translation};
	Hex h = {.c = 3, .r = 5};
	Point p = layout_hex_to_point(&l, h);

	_pt(p);
	//=> 125.981, 275
	_("%.14f", p.x);
	//=> 125.98076211353316

	f64 sqrt_3 = sqrt(3);
	_f(sqrt_3 / 2.0 * 3);
	//=> 2.598076
	_f((sqrt_3 / 2.0 * 3) * 10 + 100);
	//=> 125.980762
	_f(3.0 / 2.0 * 5);
	//=> 7.500000
	_f((3.0 / 2.0 * 5) * 10 + 200);
	//=> 275.000000
}

TEST(layout_point_to_float_hex)
{
	Point translation = {.x = -100, .y = -200};
	Layout l = {.scale = 10, .translation = translation};
	Point p = {.x = 125.981, .y = 275};
	FloatHex fh = layout_point_to_float_hex(&l, p);

	_gg(fh.c, fh.r);
	//=> 3.00003, 5

	_hx(hex_round(fh));
	//=> 3, 5
}

TEST(layout_point_to_hex)
{
	Point translation = {.x = -100, .y = -200};
	Layout l = {.scale = 10, .translation = translation};
	Point p = {.x = 125.981, .y = 275};

	_hx(layout_point_to_hex(&l, p));
	//=> 3, 5
}

TEST(point_to_hex_roundtrips)
{
	f64 scale = 10;
	Point translation = {.x = -35, .y = -71};
	Layout l = {.scale = scale, .translation = translation};
	Hex h = {.c = 3, .r = 5};

	_d(hex_equal(layout_point_to_hex(&l, layout_hex_to_point(&l, h)), h));
	//=> 1
}

TEST(layout_hex_corners)
{
	Point corners[6];
	Point translation = {.x = -100, .y = -200};
	Layout l = {.scale = 10, .translation = translation};
	Hex h = {.c = 3, .r = 5};

	_pt(layout_hex_corners(corners, &l, h)[0]);
	//=> 134.641, 280
	_pt(corners[0]);
	//=> 134.641, 280
	_pt(corners[1]);
	//=> 134.641, 270
	_pt(corners[2]);
	//=> 125.981, 265
	_pt(corners[3]);
	//=> 117.321, 270
	_pt(corners[4]);
	//=> 117.321, 280
	_pt(corners[5]);
	//=> 125.981, 285
}
