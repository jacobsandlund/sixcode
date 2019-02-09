#include "test.h"
#include "matrix.c"

TEST(mat2_multiply_v)
{
	vec2 v = {2.23, -4};
	mat2 m = {{
		{30.0, 1.0},
		{0.0, 2.5},
	}};

	_v2(mat2_multiply_v(&m, v));
	//=> 62.9, -10
}
