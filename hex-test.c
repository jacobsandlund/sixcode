#include "test.h"
#include "hex.c"

#define _hx(h) _dd(h.c, h.r)

TEST(hex_sub)
{
	Hex a = {3, 19};
	Hex b = {-42, 5};

	_hx(hex_sub(a, b));
	//=> 45, 14
}

TEST(hex_round)
{
	Hex a = {0, 0};
	Hex b = {1, -1};
	Hex c = {-1, -1};
	Hex d = {-15, 63};
	vec2 closer_to_a = {
		a.c * 0.4 + b.c * 0.3 + c.c * 0.3,
		a.r * 0.4 + b.r * 0.3 + c.r * 0.3,
	};
	vec2 closer_to_b = {
		a.c * 0.3 + b.c * 0.4 + c.c * 0.3,
		a.r * 0.3 + b.r * 0.4 + c.r * 0.3,
	};
	vec2 closer_to_c = {
		a.c * 0.3 + b.c * 0.3 + c.c * 0.4,
		a.r * 0.3 + b.r * 0.3 + c.r * 0.4,
	};
	vec2 between_c_and_d_1 = {
		a.c * 0.501 + d.c * 0.499,
		a.r * 0.501 + d.r * 0.499,
	};
	vec2 between_c_and_d_2 = {
		a.c * 0.449 + d.c * 0.501,
		a.r * 0.449 + d.r * 0.501,
	};

	_hx(hex_round(closer_to_a));
	//=> 0, 0
	_hx(hex_round(closer_to_b));
	//=> 1, -1
	_hx(hex_round(closer_to_c));
	//=> -1, -1
	_hx(hex_round(between_c_and_d_1));
	//=> -7, 31
	_hx(hex_round(between_c_and_d_2));
	//=> -8, 32
}
