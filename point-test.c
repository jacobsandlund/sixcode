#include "test.h"
#include "point.c"

#define _pt(p) _gg(p.x, p.y)

TEST(point)
{
	Point p = {.x = 3.1, .y = 150232};
	_pt(p);
	//=> 3.1, 150232
}
