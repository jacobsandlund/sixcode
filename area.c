#include <stdlib.h>
#include "area.h"

void area_zone_initialize(AreaZone *az, i32 areas_capacity)
{
	az->areas = malloc(areas_capacity * sizeof *az->areas);
	az->areas_length = 0;
	az->areas_capacity = areas_capacity;
}

void area_zone_terminate(AreaZone *az)
{
	free(az->areas);
}

static void area_grid_set_quads(AreaGrid *ag, SizeQuad *zone_size_quad, AreaQuad *quad)
{
	Quad zone_quad;
	area_quad_to_zone_quad(&zone_quad, quad);
	quad_to_size_quad(zone_size_quad, &zone_quad);
	area_zone_quad_to_area_quad(&ag->quad, &zone_quad);
}

void area_grid_initialize(AreaGrid *ag, AreaQuad *quad)
{
	area_grid_set_quads(ag, &ag->zone_quad, quad);
	i32 capacity = size_quad_capacity(&ag->zone_quad);
	ag->zones = malloc(capacity * sizeof *ag->zones);

	for (i32 i = 0; i < capacity; ++i) {
		area_zone_initialize(&ag->zones[i], AREA_ZONE_MIN_CAPACITY);
	}
}

void area_grid_terminate(AreaGrid *ag)
{
	i32 capacity = size_quad_capacity(&ag->zone_quad);

	for (i32 i = 0; i < capacity; ++i) {
		area_zone_terminate(&ag->zones[i]);
	}

	free(ag->zones);
}
