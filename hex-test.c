#include "test.h"
#include "hex.c"

#define _hx(h) _dd(h.c, h.r)
#define _fh(h) _gg(h.c, h.r)

TEST(hex)
{
	Hex h = {.c = 1, .r = 3};
	_hx(h);
	//=> 1, 3
}

TEST(hex_zero)
{
	_hx(HEX_ZERO);
	//=> 0, 0
}

TEST(frac_hex)
{
	FloatHex fh = {.c = 1.001, .r = 2.98};
	_fh(fh);
	//=> 1.001, 2.98
}

TEST(hex_equal)
{
	Hex h1 = {.c = 4, .r = 2};
	Hex h2 = {.c = 4, .r = 2};
	Hex h3 = {.c = 2, .r = 2};
	Hex h4 = {.c = 4, .r = 4};

	_d(hex_equal(h1, h1));
	//=> 1
	_d(hex_equal(h1, h2));
	//=> 1
	_d(hex_equal(h1, h3));
	//=> 0
	_d(hex_equal(h1, h4));
	//=> 0
}

TEST(hex_add)
{
	Hex a = {.c = 1, .r = -3};
	Hex b = {.c = 3, .r = -7};
	_hx(hex_add(a, b));
	//=> 4, -10
}

TEST(hex_sub)
{
	Hex a = {.c = 1, .r = -3};
	Hex b = {.c = 3, .r = -7};
	_hx(hex_sub(a, b));
	//=> -2, 4
}

TEST(hex_neighbor)
{
	Hex h = {.c = 1, .r = -3};

	_hx(hex_neighbor(h, 0));
	//=> 3, -3
	_hx(hex_neighbor(h, 1));
	//=> 2, -4
	_hx(hex_neighbor(h, 2));
	//=> 0, -4
	_hx(hex_neighbor(h, 3));
	//=> -1, -3
	_hx(hex_neighbor(h, 4));
	//=> 0, -2
	_hx(hex_neighbor(h, 5));
	//=> 2, -2
}

TEST(hex_distance)
{
	Hex a = {.c = 3, .r = -7};
	Hex b = {.c = 0, .r = -4};
	Hex c = {.c = 2, .r = 4};
	_d(hex_distance(a, b));
	//=> 3
	_d(hex_distance(hex_sub(a, b), HEX_ZERO));
	//=> 3
	_d(hex_distance(a, c));
	//=> 11
}	

TEST(hex_lerp)
{
	Hex a = {.c = 0, .r = 0};
	Hex b = {.c = 1, .r = -1};
	Hex c = {.c = 5, .r = -7};

	_fh(hex_lerp(a, b, 0.0));
	//=> 0, 0
	_fh(hex_lerp(a, b, 1.0));
	//=> 1, -1
	_fh(hex_lerp(a, b, 0.449));
	//=> 0.449, -0.449

	_fh(hex_lerp(b, c, 0.75));
	//=> 4, -5.5
}

TEST(hex_round)
{
	Hex a = {.c = 0, .r = 0};
	Hex b = {.c = 1, .r = -1};
	Hex c = {.c = -1, .r = -1};
	Hex d = {.c = 10, .r = -20};
	FloatHex closer_to_a = {
		.c = a.c * 0.4 + b.c * 0.3 + c.c * 0.3,
		.r = a.r * 0.4 + b.r * 0.3 + c.r * 0.3,
	};
	FloatHex closer_to_b = {
		.c = a.c * 0.3 + b.c * 0.4 + c.c * 0.3,
		.r = a.r * 0.3 + b.r * 0.4 + c.r * 0.3,
	};
	FloatHex closer_to_c = {
		.c = a.c * 0.3 + b.c * 0.3 + c.c * 0.4,
		.r = a.r * 0.3 + b.r * 0.3 + c.r * 0.4,
	};

	_hx(hex_round(hex_lerp(a, d, 0.51)));
	//=> 6, -10

	_d(hex_equal(hex_round(hex_lerp(a, b, 0.499)), a));
	//=> 1
	_d(hex_equal(hex_round(hex_lerp(a, b, 0.501)), b));
	//=> 1

	_d(hex_equal(hex_round(closer_to_a), a));
	//=> 1
	_d(hex_equal(hex_round(closer_to_b), b));
	//=> 1
	_d(hex_equal(hex_round(closer_to_c), c));
	//=> 1
}
