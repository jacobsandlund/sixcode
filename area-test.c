#include "test.h"
#include "area.c"
#include "area-quad.c"
#include "space.c"
#include "quad.c"

TEST(area_zone)
{
	AreaZone *az = malloc(sizeof *az);

	area_zone_initialize(az, 1024);

	_d(az->areas_length);
	//=> 0
	_d(az->areas_capacity);
	//=> 1024

	area_zone_terminate(az);

	free(az);
}

TEST(area_grid)
{
	AreaQuad area_quad = {
		.bottom_left = {-700, 44},
		.top_right = {2230, 1190},
	};
	AreaGrid *ag = malloc(sizeof *ag);

	area_grid_initialize(ag, &area_quad);

	area_grid_terminate(ag);

	free(ag);
}
