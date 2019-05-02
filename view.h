#ifndef __VIEW_H__
#define __VIEW_H__

#include "sixcode.h"
#include "hex.h"
#include "quad.h"

typedef struct {
	vec2 viewport_size;
	vec2 translation;
	float scale;
} View;

vec2 view_screen_to_world(View *vw, vec2 v);
vec2 view_world_to_hex(vec2 v);
vec2 view_hex_to_world(vec2 v);
void view_zoom_at_screen_point(View *vw, vec2 v, float new_scale);
void view_resize(View *vw, vec2 viewport_size);
void view_translate(View *vw, vec2 delta);
void view_viewport_to_quad(View *vw, Quad *out_q);
void view_screen_points_to_quad(View *vw, Quad *out_q, vec2 v1, vec2 v2);

#endif // __VIEW_H__
