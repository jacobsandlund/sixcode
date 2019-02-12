#ifndef __VIEW_H__
#define __VIEW_H__

#include "hex.h"
#include "matrix.h"
#include "sixcode.h"
#include "quad.h"

typedef struct {
	vec2 viewport_size;
	vec2 translation;
	f32 scale;
} View;

void view_zoom_at_point(View *vw, vec2 v, f32 new_scale);
void view_resize(View *vw, vec2 viewport_size);
void view_translate(View *vw, vec2 delta);
vec2 view_point_to_hex_space(View *vw, vec2 v);
void view_viewport_to_quad(View *vw, Quad *out_q);

#endif // __VIEW_H__
