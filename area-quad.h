#ifndef __AREA_QUAD_H__
#define __AREA_QUAD_H__

#include "sixcode.h"
#include "quad.h"
#include "vector.h"

typedef struct {
	vec2 bottom_left;
	vec2 bottom_right;
	vec2 top_left;
	vec2 top_right;
} AreaQuad;

void area_quad_from_quad(AreaQuad *aq, Quad *quad);
void area_quad_to_zone_quad(Quad *zone_quad, AreaQuad *aq);
void area_zone_quad_to_area_quad(AreaQuad *aq, Quad *zone_quad);

#endif // __AREA_QUAD_H__
