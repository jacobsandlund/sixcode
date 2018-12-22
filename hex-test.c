#include "test.h"
#include "hex.c"

#define _hx(h) _dd(h.q, h.r)
#define _fh(h) _gg(h.q, h.r)

TEST(hex)
{
	Hex h = {.q = 1, .r = 2};
	_hx(h);
	//=> 1, 2
}

TEST(frac_hex)
{
	FloatHex fh = {.q = 1.001, .r = 2.98};
	_fh(fh);
	//=> 1.001, 2.98
}

TEST(hex_s)
{
	Hex h = {.q = 3, .r = -2};
	_d(hex_s(h));
	//=> -1
}

TEST(hex_equal)
{
	Hex h1 = {.q = 3, .r = 2};
	Hex h2 = {.q = 3, .r = 2};
	Hex h3 = {.q = 2, .r = 2};
	Hex h4 = {.q = 3, .r = 3};

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
	Hex a = {.q = 1, .r = -3};
	Hex b = {.q = 3, .r = -7};
	_hx(hex_add(a, b));
	//=> 4, -10
}

TEST(hex_subtract)
{
	Hex a = {.q = 1, .r = -3};
	Hex b = {.q = 3, .r = -7};
	_hx(hex_subtract(a, b));
	//=> -2, 4
}

TEST(hex_neighbor)
{
	Hex h = {.q = 1, .r = -2};

	_hx(hex_neighbor(h, 0));
	//=> 2, -2
	_hx(hex_neighbor(h, 1));
	//=> 2, -3
	_hx(hex_neighbor(h, 2));
	//=> 1, -3
	_hx(hex_neighbor(h, 3));
	//=> 0, -2
	_hx(hex_neighbor(h, 4));
	//=> 0, -1
	_hx(hex_neighbor(h, 5));
	//=> 1, -1
}

TEST(hex_distance)
{
	Hex a = {.q = 3, .r = -7};
	Hex b = {.q = -1, .r = -4};
	Hex zero = {.q = 0, .r = 0};
	_g(hex_distance(a, b));
	//=> 4
	_g(hex_distance(hex_subtract(a, b), zero));
	//=> 4
}	

TEST(hex_lerp)
{
	Hex a = {.q = 0, .r = 0};
	Hex b = {.q = 1, .r = -1};
	Hex c = {.q = 4, .r = -7};

	_fh(hex_lerp(a, b, 0.0));
	//=> 0, 0
	_fh(hex_lerp(a, b, 1.0));
	//=> 1, -1
	_fh(hex_lerp(a, b, 0.449));
	//=> 0.449, -0.449

	_fh(hex_lerp(b, c, 0.75));
	//=> 3.25, -5.5
}

TEST(hex_round)
{
	Hex a = {.q = 0, .r = 0};
	Hex b = {.q = 1, .r = -1};
	Hex c = {.q = 0, .r = -1};
	Hex d = {.q = 10, .r = -20};
	FloatHex closer_to_a = {
		.q = a.q * 0.4 + b.q * 0.3 + c.q * 0.3,
		.r = a.r * 0.4 + b.r * 0.3 + c.r * 0.3,
	};
	FloatHex closer_to_b = {
		.q = a.q * 0.3 + b.q * 0.4 + c.q * 0.3,
		.r = a.r * 0.3 + b.r * 0.4 + c.r * 0.3,
	};
	FloatHex closer_to_c = {
		.q = a.q * 0.3 + b.q * 0.3 + c.q * 0.4,
		.r = a.r * 0.3 + b.r * 0.3 + c.r * 0.4,
	};

	_hx(hex_round(hex_lerp(a, d, 0.51)));
	//=> 5, -10

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
