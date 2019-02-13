#include "test.h"
#include "space.c"
#include "hex.c"

TEST(space_hex_round)
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

	_hx(space_hex_round(closer_to_a));
	//=> 0, 0
	_hx(space_hex_round(closer_to_b));
	//=> 1, -1
	_hx(space_hex_round(closer_to_c));
	//=> -1, -1
	_hx(space_hex_round(between_c_and_d_1));
	//=> -7, 31
	_hx(space_hex_round(between_c_and_d_2));
	//=> -8, 32
}

TEST(space_hex_floor)
{
	vec2 v = {-3.1, 17.6};
	_hx(space_hex_floor(v));
	//=> -4, 17
}

TEST(space_hex_to_vec)
{
	Hex h = {-3, 17};
	_v2(space_hex_to_vec(h));
	//=> -3, 17
}

TEST(space_world_to_hex)
{
	vec2 v1 = {-94.8038, 45};
	vec2 v2 = {76.06, -8};

	_v2(space_world_to_hex(v1));
	//=> -109.47, -30

	_v2(space_world_to_hex(v2));
	//=> 87.8265, 5.33333
}

TEST(space_hex_to_world)
{
	vec2 v1 = {-109.47, 30};
	vec2 v2 = {87.8265, 5.333333333};

	_v2(space_hex_to_world(v1));
	//=> -94.8038, -45

	_v2(space_hex_to_world(v2));
	//=> 76.06, -8
}

TEST(space_hex_to_storage)
{
	Hex h1 = {-5, 43};
	Hex h2 = {64, -28};

	_hx(space_hex_to_storage(h1));
	//=> -3, 43
	_hx(space_hex_to_storage(h2));
	//=> 32, -28
}

TEST(space_storage_to_hex)
{
	Hex h1 = {-3, 43};
	Hex h2 = {32, -28};

	_hx(space_storage_to_hex(h1));
	//=> -5, 43
	_hx(space_storage_to_hex(h2));
	//=> 64, -28
}

TEST(space_world_to_area_zone)
{
	vec2 v1 = {-400.5, 1220};
	vec2 v2 = {-7680, 767};

	_v2(space_world_to_area_zone(v1));
	//=> -0.521484, 1.58854
	_v2(space_world_to_area_zone(v2));
	//=> -10, 0.998698
}

TEST(space_area_zone_to_world)
{
	vec2 v1 = {-0.521484, 1.58854};
	vec2 v2 = {-10, 0.998698};

	_v2(space_area_zone_to_world(v1));
	//=> -400.5, 1220
	_v2(space_area_zone_to_world(v2));
	//=> -7680, 767
}
