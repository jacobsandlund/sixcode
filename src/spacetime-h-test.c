#include "test.h"
#include "spacetime.h"

TEST(vec2_ivec2_conversions)
{
	vec2 v = {3.1, -42.7};
	_v2(v);
	//=> 3.1, -42.7

	ivec2 iv = ivec2_from_vec(v);
	_i2(iv);
	//=> 3, -42

	_v2(vec2_from_ivec(iv));
	//=> 3, -42
}

TEST(ivec2_math)
{
	ivec2 v1 = {3, -7};
	ivec2 v2 = {5, 11};

	_i2(ivec2_add(v1, v2));
	//=> 8, 4
	_i2(ivec2_sub(v2, v1));
	//=> 2, 18
}
