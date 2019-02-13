#include "test.h"
#include "hex.c"

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
