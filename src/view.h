#ifndef VIEW_H
#define VIEW_H

#include "spacetime.h"
#include "quad.h"

typedef enum {
	VIEW_LAYOUT_HEX = 0,
	VIEW_LAYOUT_RECT = 1,
} ViewLayout;

#define VIEW_NUM_LAYOUTS 2

typedef struct {
	float4x4 view_matrix;
	float2 viewport_size;
	float2 translation;
	double2 scale;
	float layout_independent_scale;
	ViewLayout layout;
} View;

void view_initialize(View *vw, float2 viewport_size, float2 translation, float layout_independent_scale);
void view_layout(View *vw, ViewLayout layout);
float2 view_screen_to_world(View *vw, float2 v);
void view_zoom_at_screen_point(View *vw, float2 v, float new_scale);
void view_resize(View *vw, float2 viewport_size);
void view_translate(View *vw, float2 delta);
void view_update_matrix(View *vw, float2 draw_offset);
int2 view_world_round(View *vw, float2 v);
void view_viewport_to_quad(View *vw, Quad *out_q);

#endif // VIEW_H
