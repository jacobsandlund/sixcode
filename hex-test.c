#include "hex.c"
#include "test.h"

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

TEST(hex_floor)
{
	vec2 v = {-3.1, 17.6};
	_hx(hex_floor(v));
	//=> -4, 17
}

TEST(hex_to_vec)
{
	Hex h = {-3, 17};
	_v2(hex_to_vec(h));
	//=> -3, 17
}

TEST(hex_to_storage)
{
	Hex h1 = {-5, 43};
	Hex h2 = {64, -28};

	_hx(hex_to_storage(h1));
	//=> -3, 43
	_hx(hex_to_storage(h2));
	//=> 32, -28
}

TEST(hex_from_storage)
{
	Hex h1 = {-3, 43};
	Hex h2 = {32, -28};

	_hx(hex_from_storage(h1));
	//=> -5, 43
	_hx(hex_from_storage(h2));
	//=> 64, -28
}

TEST(hex_add)
{
	Hex a = {3, 19};
	Hex b = {-42, 5};

	_hx(hex_add(a, b));
	//=> -39, 24
}

TEST(hex_sub)
{
	Hex a = {3, 19};
	Hex b = {-42, 5};

	_hx(hex_sub(a, b));
	//=> 45, 14
}
