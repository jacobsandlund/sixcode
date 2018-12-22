#include "test.h"
#include "point.c"

TEST(point)
{
	Point p = {.x = 3.1, .y = 150232};
	_gg(p.x, p.y);
	//=> 3.1, 150232
}
