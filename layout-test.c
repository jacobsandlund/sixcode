#include "test.h"
#include "hex.c"
#include "point.c"
#include "layout.c"

#define _pt(p) _gg(p.x, p.y)
#define _hx(h) _dd(h.q, h.r)

TEST(layout_pointy)
{
	_("%.16f", LAYOUT_POINTY.f[0]);
	//=> 1.7320508075688772
	_("%.16f", LAYOUT_POINTY.f[1]);
	//=> 0.8660254037844386
	_("%.16f", LAYOUT_POINTY.f[2]);
	//=> 0.0000000000000000
	_("%.16f", LAYOUT_POINTY.f[3]);
	//=> 1.5000000000000000
	_("%.16f", LAYOUT_POINTY.b[0]);
	//=> 0.5773502691896257
	_("%.16f", LAYOUT_POINTY.b[1]);
	//=> -0.3333333333333333
	_("%.16f", LAYOUT_POINTY.b[2]);
	//=> 0.0000000000000000
	_("%.16f", LAYOUT_POINTY.b[3]);
	//=> 0.6666666666666666
	_g(LAYOUT_POINTY.start_angle);
	//=> 0.5
}

TEST(layout_flat)
{
	_("%.16f", LAYOUT_FLAT.f[0]);
	//=> 1.5000000000000000
	_("%.16f", LAYOUT_FLAT.f[1]);
	//=> 0.0000000000000000
	_("%.16f", LAYOUT_FLAT.f[2]);
	//=> 0.8660254037844386
	_("%.16f", LAYOUT_FLAT.f[3]);
	//=> 1.7320508075688772
	_("%.16f", LAYOUT_FLAT.b[0]);
	//=> 0.6666666666666666
	_("%.16f", LAYOUT_FLAT.b[1]);
	//=> 0.0000000000000000
	_("%.16f", LAYOUT_FLAT.b[2]);
	//=> -0.3333333333333333
	_("%.16f", LAYOUT_FLAT.b[3]);
	//=> 0.5773502691896257
	_g(LAYOUT_FLAT.start_angle);
	//=> 0
}

TEST(layout)
{
	Point size = {.x = 15, .y = 20};
	Point origin = {.x = 100, .y = 250};
	Layout l = {
		.orientation = LAYOUT_POINTY,
		.size = size,
		.origin = origin,
	};

	_g(l.orientation.start_angle);
	//=> 0.5
	_f(l.orientation.f[0]);
	//=> 1.732051
	_pt(l.size);
	//=> 15, 20
	_pt(l.origin);
	//=> 100, 250
}

TEST(layout_hex_to_point)
{
	Point size = {.x = 10, .y = 20};
	Point origin = {.x = 100, .y = 200};
	Layout l = {.orientation = LAYOUT_POINTY, .size = size, .origin = origin};
	Hex h = {.q = 3, .r = 4};
	Point p = layout_hex_to_point(&l, h);

	_pt(p);
	//=> 186.603, 320
	_("%.14f", p.x);
	//=> 186.60254037844385

	_f(SQRT_3 * 3 + SQRT_3 / 2.0 * 4);
	//=> 8.660254
	_f((SQRT_3 * 3 + SQRT_3 / 2.0 * 4) * 10 + 100);
	//=> 186.602540
	_f(0 * 3 + 3.0 / 2.0 * 4);
	//=> 6.000000
	_f((0 * 3 + 3.0 / 2.0 * 4) * 20 + 200);
	//=> 320.000000
}

TEST(layout_point_to_hex)
{
	Point size = {.x = 10, .y = 20};
	Point origin = {.x = 100, .y = 200};
	Layout l = {.orientation = LAYOUT_POINTY, .size = size, .origin = origin};
	Point p = {.x = 186.6, .y = 320};
	FloatHex fh = layout_point_to_hex(&l, p);

	_gg(fh.q, fh.r);
	//=> 2.99985, 4

	_hx(hex_round(fh));
	//=> 3, 4
}

TEST(point_to_hex_roundtrips)
{
	Point size = {.x = 10, .y = 15};
	Point origin = {.x = 35, .y = 71};
	Layout pointy = {.orientation = LAYOUT_POINTY, .size = size, .origin = origin};
	Layout flat = {.orientation = LAYOUT_FLAT, .size = size, .origin = origin};
	Hex h = {.q = 3, .r = 4};

	_d(hex_equal(hex_round(layout_point_to_hex(&pointy, layout_hex_to_point(&pointy, h))), h));
	//=> 1

	_d(hex_equal(hex_round(layout_point_to_hex(&flat, layout_hex_to_point(&flat, h))), h));
	//=> 1
}

TEST(layout_hex_corners)
{
	Point corners[6];
	Point size = {.x = 10, .y = 20};
	Point origin = {.x = 100, .y = 200};
	Layout l = {.orientation = LAYOUT_POINTY, .size = size, .origin = origin};
	Hex h = {.q = 3, .r = 4};

	_pt(layout_hex_corners(corners, &l, h)[0]);
	//=> 195.263, 330
	_pt(corners[0]);
	//=> 195.263, 330
	_pt(corners[1]);
	//=> 195.263, 310
	_pt(corners[2]);
	//=> 186.603, 300
	_pt(corners[3]);
	//=> 177.942, 310
	_pt(corners[4]);
	//=> 177.942, 330
	_pt(corners[5]);
	//=> 186.603, 340
}
