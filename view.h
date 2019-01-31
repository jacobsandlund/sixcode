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
void view_update_matrix(View *vw);
void view_zoom_at_point(View *vw, vec2 v, f32 new_scale);
void view_resize(View *vw, vec2 viewport_size);
Hex view_point_to_hex(View *vw, vec2 vec_hex);

#endif // __VIEW_H__
