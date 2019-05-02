#include "sixcode.h"
#include "test.h"

TEST(conversions)
{
	vec2 v = {3.0, 4.5};
	_v2(v);
	//=> 3, 4.5
	ivec2 iv = ivec2_from_vec(v);
	_i2(iv);
	//=> 3, 4

	_v2(vec2_from_ivec(iv));
	//=> 3, 4
}
