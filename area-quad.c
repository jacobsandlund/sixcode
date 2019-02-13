#include "area-quad.h"
#include "space.h"

void area_quad_from_quad(AreaQuad *aq, Quad *quad)
{
	vec2 top_left = space_hex_to_world(space_hex_to_vec(quad->min));
	vec2 bottom_right = space_hex_to_world(space_hex_to_vec(quad->max));
	aq->bottom_left = (vec2) {top_left.x, bottom_right.y};
	aq->bottom_right = bottom_right;
	aq->top_left = top_left;
	aq->top_right = (vec2) {bottom_right.x, top_left.y};
}

void area_quad_to_zone_quad(Quad *zone_quad, AreaQuad *aq)
{
	zone_quad->min = space_hex_floor(
			space_world_to_area_zone(aq->bottom_left));
	zone_quad->max = space_hex_floor(
			space_world_to_area_zone(aq->top_right));
}

void area_zone_quad_to_area_quad(AreaQuad *aq, Quad *zone_quad)
{
	aq->bottom_left = space_area_zone_to_world(
			space_hex_to_vec(zone_quad->min));

	vec2 max = {zone_quad->max.c + 1, zone_quad->max.r + 1};
	aq->top_right = space_area_zone_to_world(max);
	aq->bottom_right = (vec2) {aq->top_right.x, aq->bottom_left.y};
	aq->top_left = (vec2) {aq->bottom_left.x, aq->top_right.y};
}
