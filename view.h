#ifndef __VIEW_H__
#define __VIEW_H__

#include "sixcode.h"
#include "quad.h"

typedef enum {
	VIEW_LAYOUT_HEX,
	VIEW_LAYOUT_RECT,
} ViewLayout;

typedef struct {
	mat4 view_matrix;
	vec2 viewport_size;
	vec2 translation;
	dvec2 scale;
	float layout_independent_scale;
	ViewLayout layout;
} View;

void view_initialize(View *vw, vec2 viewport_size, vec2 translation, float layout_independent_scale);
void view_layout(View *vw, ViewLayout layout);
vec2 view_screen_to_world(View *vw, vec2 v);
void view_zoom_at_screen_point(View *vw, vec2 v, float new_scale);
void view_resize(View *vw, vec2 viewport_size);
void view_translate(View *vw, vec2 delta);
void view_update_matrix(View *vw, vec2 draw_offset);
ivec2 view_hex_round(vec2 v);
void view_viewport_to_quad(View *vw, Quad *out_q);
void view_screen_points_to_quad(View *vw, Quad *out_q, vec2 v1, vec2 v2);

#endif // __VIEW_H__
