#ifndef __VIEW_H__
#define __VIEW_H__

#include "hex.h"
#include "matrix.h"
#include "sixcode.h"

typedef struct {
	mat4 view_matrix;
	vec2 viewport_size;
	vec2 translation;
	f32 scale;
} View;

void view_initialize(View *vw, vec2 viewport_size, vec2 translation, f32 scale);

void view_zoom_at_point(View *vw, vec2 v, f32 new_scale);
void view_resize(View *vw, vec2 viewport_size);
EXPORT_WEB void view_update_matrix(View *vw);

vec2 view_hex_to_point(View *vw, Hex h);
FloatHex view_point_to_float_hex(View *vw, vec2 v);

//FloatQuad view_point_quad_to_float_quad(View *vw, PointQuad pq);
//PointQuad view_float_quad_to_point_quad(View *vw, FloatQuad fq);

#endif // __VIEW_H__
