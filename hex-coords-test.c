#include "hex-coords.c"
#include "test.h"

TEST(hex_coords_round)
{
	ivec2 a = {0, 0};
	ivec2 b = {1, -1};
	ivec2 c = {-1, -1};
	ivec2 d = {-15, 63};
	vec2 closer_to_a = {
		a.x * 0.4 + b.x * 0.3 + c.x * 0.3,
		a.y * 0.4 + b.y * 0.3 + c.y * 0.3,
	};
	vec2 closer_to_b = {
		a.x * 0.3 + b.x * 0.4 + c.x * 0.3,
		a.y * 0.3 + b.y * 0.4 + c.y * 0.3,
	};
	vec2 closer_to_c = {
		a.x * 0.3 + b.x * 0.3 + c.x * 0.4,
		a.y * 0.3 + b.y * 0.3 + c.y * 0.4,
	};
	vec2 between_c_and_d_1 = {
		a.x * 0.501 + d.x * 0.499,
		a.y * 0.501 + d.y * 0.499,
	};
	vec2 between_c_and_d_2 = {
		a.x * 0.449 + d.x * 0.501,
		a.y * 0.449 + d.y * 0.501,
	};

	_i2(hex_coords_round(closer_to_a));
	//=> 0, 0
	_i2(hex_coords_round(closer_to_b));
	//=> 1, -1
	_i2(hex_coords_round(closer_to_c));
	//=> -1, -1
	_i2(hex_coords_round(between_c_and_d_1));
	//=> -7, 31
	_i2(hex_coords_round(between_c_and_d_2));
	//=> -8, 32
}

TEST(hex_coords_to_storage)
{
	ivec2 h1 = {-5, 43};
	ivec2 h2 = {64, -28};

	_i2(hex_coords_to_storage(h1));
	//=> -3, 43
	_i2(hex_coords_to_storage(h2));
	//=> 32, -28
}

TEST(hex_coords_from_storage)
{
	ivec2 h1 = {-3, 43};
	ivec2 h2 = {32, -28};

	_i2(hex_coords_from_storage(h1));
	//=> -5, 43
	_i2(hex_coords_from_storage(h2));
	//=> 64, -28
}
