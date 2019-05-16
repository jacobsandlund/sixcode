#ifndef __VIEW_H__
#define __VIEW_H__

#include "6code.h"
#include "quad.h"

typedef enum {
	VIEW_LAYOUT_HEX = 0,
	VIEW_LAYOUT_RECT = 1,
} ViewLayout;

#define VIEW_NUM_LAYOUTS 2

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
ivec2 view_world_round(View *vw, vec2 v);
void view_viewport_to_quad(View *vw, Quad *out_q);

#endif // __VIEW_H__
