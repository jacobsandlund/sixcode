#include "test.h"
#include "area-quad.c"
#include "space.c"
#include "quad.c"

TEST(area_quad_from_quad)
{
	AreaQuad aq;
	Quad q = {{-45, 15}, {33, 87}};

	area_quad_from_quad(&aq, &q);
	_aq(aq);
	//=> (-38.9711, -22.5), (28.5788, -22.5)
	//=> (-38.9711, -130.5), (28.5788, -130.5)
}

TEST(area_quad_to_zone_quad)
{
	Quad zone_quad;
	AreaQuad aq = {
		.bottom_left = {-38.9711, -1190.9},
		.top_right = {28.5788, 2253},
	};

	area_quad_to_zone_quad(&zone_quad, &aq);

	_qd(zone_quad);
	//=> (-1, -2), (0, 2)
}

TEST(area_zone_quad_to_area_quad)
{
	AreaQuad aq;
	Quad zone_quad = {{-1, -2}, {0, 2}};

	area_zone_quad_to_area_quad(&aq, &zone_quad);

	_aq(aq);
	//=> (-768, 2304), (768, 2304)
	//=> (-768, -1536), (768, -1536)
}
